git submodule update --init --recursive
rm -rf embuild
docker build -t h264-mp4-encoder .
docker run --name h264-mp4-encoder-temp h264-mp4-encoder /bin/true
docker cp h264-mp4-encoder-temp:/home/user/embuild embuild
docker rm h264-mp4-encoder-temp
