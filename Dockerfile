FROM node:carbon-alpine3.9
ADD docker_files/exec.sh /workspace/
ADD wait/build/file_wait /workspace/
WORKDIR /workspace
ENTRYPOINT ["./exec.sh"]