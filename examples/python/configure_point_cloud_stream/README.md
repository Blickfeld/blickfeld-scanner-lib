# Configure pointcloud stream

Fetch a basic point cloud. Only get specific fields of each point: X, Y, Z, intensity and ID.

Use a filter to filter out points which are closer than 5m or further away than 50m.

```.. literalinclude:: configure_point_cloud_stream.py
    :language: python
```
