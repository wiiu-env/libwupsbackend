FROM wiiuenv/devkitppc:20210920

COPY --from=wiiuenv/wiiupluginsystem:20210924 /artifacts $DEVKITPRO

WORKDIR tmp_build
COPY . .
RUN make clean && make && mkdir -p /artifacts/wups && cp -r lib /artifacts/wups && cp -r include /artifacts/wups && cp -r share /artifacts/wups
WORKDIR /artifacts

FROM scratch
COPY --from=0 /artifacts /artifacts