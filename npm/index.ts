/*
 * Copyright (c) 2020 Blickfeld GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE.md file in the root directory of this source tree.
 */
import * as net from 'net';
import * as Queue from 'promise-queue';

import * as connection_pb from './blickfeld/connection_pb';
import * as scan_pattern_pb from './blickfeld/config/scan_pattern_pb';
import * as error_pb from './blickfeld/error_pb';


export class Scanner {
    hostname: string;
    port: number;
    connection: Connection;

    constructor(hostnameOrIp='localhost', port=8000) {
        this.hostname = hostnameOrIp;
        this.port = port;
        this.connection = this.createConnection();

    }

    createConnection() {
        return new Connection(this.hostname, this.port);
    }

    getSetTrajectory() {
        const req = new connection_pb.Request();
        const getScanPattern = new connection_pb.Request.GetScanPattern();
        req.setGetScanPattern(getScanPattern);
        return this.connection.sendRequest(req);
    }

    setScanPattern(config: scan_pattern_pb.ScanPattern, persist = false) {
        const req = new connection_pb.Request();
        const setScanPattern = new connection_pb.Request.SetScanPattern();
        setScanPattern.setConfig(config);
        setScanPattern.setPersist(persist);
        req.setSetScanPattern(setScanPattern);
        return this.connection.sendRequest(req);
    }

    fillScanPattern(config: scan_pattern_pb.ScanPattern) {
        const req = new connection_pb.Request();
        const fillScanPattern = new connection_pb.Request.FillScanPattern();
        fillScanPattern.setConfig(config);
        req.setFillScanPattern(fillScanPattern);
        return this.connection.sendRequest(req).then(resp => resp.getFillScanPattern()!.getConfig()!);
    }

    upgradeLegacyScanPattern(config: scan_pattern_pb.ScanPattern) {
        const req = new connection_pb.Request();
        const fillScanPattern = new connection_pb.Request.FillScanPattern();
        fillScanPattern.setLegacyConfig(config);
        req.setFillScanPattern(fillScanPattern);
        return this.connection.sendRequest(req).then(resp => resp.getFillScanPattern()!.getConfig()!);
    }

    getStatus() {
        const req = new connection_pb.Request();
        const status = new connection_pb.Request.Status();
        req.setStatus(status);
        return this.connection.sendRequest(req).then(resp => resp.getStatus()!);
    }
    
    json(reqObject: Object, acceptFormat: connection_pb.FormatMap[keyof connection_pb.FormatMap] = connection_pb.Format.JSON) : Promise<Object | connection_pb.Response> {
    	const req = new connection_pb.Request();
    	req.setAsjson(JSON.stringify(reqObject));
    	req.setAcceptFormat(acceptFormat);
        return this.connection.sendRequest(req).then(resp => acceptFormat == connection_pb.Format.JSON ? JSON.parse(resp.getAsjson()!) : resp);
    }
    
    convertScanPatternToJSON(obj: scan_pattern_pb.ScanPattern): Promise<Object> {
    	const req = new connection_pb.Request();
        const fillScanPattern = new connection_pb.Request.FillScanPattern();
        fillScanPattern.setConfig(obj);
        req.setFillScanPattern(fillScanPattern);
    	req.setAcceptFormat(connection_pb.Format.JSON);
        return this.connection.sendRequest(req).then(resp => JSON.parse(resp.getAsjson()!)["fill_scan_pattern"]["config"]);
    }
    
    convertScanPatternToPB(obj: Object): Promise<scan_pattern_pb.ScanPattern> {
    	const req = new connection_pb.Request();
    	req.setAsjson(JSON.stringify({ "fill_scan_pattern": { "config": obj } }));
    	req.setAcceptFormat(connection_pb.Format.PROTOBUF);
        return this.connection.sendRequest(req).then(resp => resp.getFillScanPattern()!.getConfig()!);
    }
    
}

class ProtocolException extends Error {
    errno: number;
    constructor(error: error_pb.Error | undefined) {
        super();
        this.errno = 0;
        if(error) {
            this.errno = error.getErrorCase();
            const parsedError = JSON.parse(JSON.stringify(error.toObject()));
            const keys = Object.keys(parsedError);
            keys.forEach((key: string) => {
               if(parsedError[key]) {
                   this.name =  key;
                   this.message = JSON.stringify(parsedError[key]);
               }
            });
        };
    }
}

class Connection {
    socket: net.Socket;
    hostnameOrIp: string;
    port: number;
    connected: boolean;

    private listeners: Array<{ error: (error: ProtocolException) => void, success: (resp: connection_pb.Response) => void}> = [];
    private buffer = Buffer.alloc(0);
    private expected_header = true;;
    private expected_len = 4;

    constructor(hostnameOrIp: string, port: number) {
        this.socket = new net.Socket();
        this.hostnameOrIp = hostnameOrIp;
        this.port = port;
        this.connected = false;
    }

    connect() {
        return new Promise((resolve, reject) => {
            const err_cb = (err: Error) => {
                if (!this.connected)
                    return reject(err);
                this.connected = false;
            };

            this.socket = net.createConnection(this.port, this.hostnameOrIp, () => {
                this.socket.off('error', err_cb);
                this.socket.on('error', (error: Error) => this.errorHandler(error));
                this.connected = true;

                const req = new connection_pb.Request();
                req.setHello(new connection_pb.Request.Hello());
                req.getHello()!.setProtocolVersion(1);

                return this.sendRequest(req).then(() => {
                    return resolve("Connected to " + this.hostnameOrIp + ':' + this.port);
                }).catch(err => {
                    return reject(err);
                });
            });

            this.socket.on('data', (data: Buffer) => this.dataHandler(data));
            this.socket.once('error', err_cb);
        })
    }

    del() {
        this.socket.destroy();
    }

    dataHandler(data: Buffer) {
        this.buffer = Buffer.concat([this.buffer, data]);
        while (this.buffer.length >= this.expected_len) {
            if (this.expected_header) {
                this.expected_len = this.buffer.readUInt32LE(0);
                this.buffer = this.buffer.slice(4);
            } else {
                let resp = connection_pb.Response.deserializeBinary(this.buffer.slice(0, this.expected_len));
                this.buffer = this.buffer.slice(this.expected_len);
                this.expected_len = 4;

                if (this.listeners.length) {
                    const listener = this.listeners.pop()!;
                    if (resp.hasError()) {
                        listener.error(new ProtocolException(resp.getError()));
                    } else {
                        listener.success(resp);
                    }
                } else {
                    // TODO Handle unexpected message
                }
            }
            this.expected_header = !this.expected_header;
        }
    }

    errorHandler(error: Error) {
        
    }

    private sendRequestQueue = new Queue(1, 10);
    sendRequest(req: connection_pb.Request): Promise<connection_pb.Response> {
        return this.sendRequestQueue.add(() => {
            return this.send(req).then(() => {
                return this.recv();
            });
        });
    }

    send(req: connection_pb.Request): Promise<string> {
        return new Promise((resolve, reject) => {
            const data = req.serializeBinary();
            let lenData = Buffer.alloc(4);
            lenData.writeUInt32LE(data.length, 0);
            if (this.socket.writable) {
                this.socket.write(lenData, () => {
                    this.socket.write(data, () => {
                        return resolve('Data successfully written');
                    });
                });
            } else {
                return reject('Connection to BSS failed.');
            }
        });
    }

    recv(): Promise<connection_pb.Response> {
        return new Promise((resolve, reject) => {
            this.listeners.push({ error: reject, success: resolve });
        });
    }
}
