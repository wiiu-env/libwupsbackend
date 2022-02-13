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
COPY --from=wiiuenv/libwupsbackend:[tag] /artifacts $DEVKITPRO
[...]
```
Replace [tag] with a tag you want to use, a list of tags can be found [here](https://hub.docker.com/r/wiiuenv/libwupsbackend/tags).
It's highly recommended to pin the version to the **latest date** instead of using `latest`.