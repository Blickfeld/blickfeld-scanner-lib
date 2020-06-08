import os
import requests
from collections import namedtuple

from github import Github, GitRelease
from git import Repo
from fetch_artifacts import fetch_artifacts

local_repo = Repo()

g = Github(os.environ['GITHUB_TOKEN'])
remote_repo = g.get_repo("Blickfeld/blickfeld-scanner-lib")

# Iterate through all tags
for tag in local_repo.tags:
    release_tag = tag.name

    # CreateOrFetch release from GitHub API
    try:
        release = remote_repo.get_release(release_tag)
        
        if release.body:
            print(f"Skipping {release_tag} as release notes are already set")
            continue
    except:
        print("Create release for", release_tag)
        headers, data = remote_repo._requester.requestJsonAndCheck("POST", remote_repo.url + "/releases", input = {
            "tag_name": release_tag,
            "prerelease": True,
        })
        
        release = GitRelease.GitRelease(remote_repo._requester, headers, data, completed=True)
        
    # Release notes
    message = local_repo.tags[release_tag].commit.message.split("\n",2)[2]
        
    print("Updating release", release_tag)
    print(message)
    release.update_release(name=release_tag, message=message)

    try:
        # Fetch artifacts
        args = { 
            "artifacts": [open("./.ci/artifacts.yml", 'r')],
            "only_clones": False,
            "only_artifacts": False,
            "write_versions": None,
            "write_env_scripts": None
        }
        os.environ['BF_ARTIFACTS_OVERWRITE_ENDUSER_SOFTWARE_BLICKFELD_SCANNER_LIB'] = release_tag
        fetch_artifacts(namedtuple('Args', args.keys())(*args.values()))
        
        # Upload package to PyPI
        os.system(f"twine upload --username __token__ --password {os.environ['PIP_TOKEN']} build_amd64_python/python/dist/blickfeld_scanner-{release_tag[1:]}.tar.gz")

        # Delete existing assets
        for asset in release.get_assets():
            asset.delete_asset()
        
        # Upload assets
        release.upload_asset("build/blickfeld-scanner-lib-dev-Linux.deb")
        release.upload_asset("build/blickfeld-scanner-lib-dev-standalone-Linux.deb")
        release.upload_asset("build/blickfeld-scanner-lib-dev-testing-Linux.deb")
        release.upload_asset("build_amd64_python/python/dist/blickfeld_scanner.tar.gz", name="python_blickfeld_scanner.tar.gz")   
    except:
        print(f"Failed to upload artifacts for {release_tag}.")
