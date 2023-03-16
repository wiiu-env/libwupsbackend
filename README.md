[![Publish Docker Image](https://github.com/wiiu-env/libwupsbackend/actions/workflows/push_image.yml/badge.svg)](https://github.com/wiiu-env/libwupsbackend/actions/workflows/push_image.yml)

## libwupsbackend
Requires the [WiiUPluginLoaderBackendModule](https://github.com/wiiu-env/WiiUPluginLoaderBackend) to be running via [WUMSLoader](https://github.com/wiiu-env/WUMSLoader).
Requires [wut](https://github.com/devkitpro/wut) for building.
Install via `make install`.

## Building
Make you to have [wut](https://github.com/devkitPro/wut/) installed and use the following command for build:
```
make install
```

## Use this lib in Dockerfiles.
A prebuilt version of this lib can found on dockerhub. To use it for your projects, add this to your Dockerfile.
```
[...]
COPY --from=ghcr.io/wiiu-env/libwupsbackend:[tag] /artifacts $DEVKITPRO
[...]
```
Replace [tag] with a tag you want to use, a list of tags can be found [here](https://github.com/wiiu-env/libkernel/pkgs/container/libwupsbackend/versions).
It's highly recommended to pin the version to the **latest date** instead of using `latest`.

## Format the code via docker

`docker run --rm -v ${PWD}:/src ghcr.io/wiiu-env/clang-format:13.0.0-2 -r ./source ./include -i`