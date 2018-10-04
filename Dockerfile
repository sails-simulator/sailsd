FROM quay.io/sails/libsailing:latest as libsailing

FROM alpine:latest as build

RUN apk update ;\
    apk add build-base jansson-dev

WORKDIR /sailsd

COPY . ./
COPY --from=libsailing /usr/local/lib/libsailing/*.o libsailing/
COPY --from=libsailing /usr/local/include/libsailing/*.h libsailing/

# Bind on all adapters so dockers bridge network works as expected
RUN sed -i.bak 's/127\.0\.0\.1/0\.0\.0\.0/g' sailsd.c

RUN make sailsd install

FROM alpine:latest

RUN apk update && apk add jansson-dev

COPY --from=build /usr/local/bin/sailsd /usr/local/bin/sailsd

EXPOSE 3333

ENV PYTHONUNBUFFERED 1

ENTRYPOINT ["/bin/sh", "-c", "/usr/local/bin/sailsd"]