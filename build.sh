git submodule update --init --recursive
rm -rf embuild
docker build -t h264_mp4_encoder .
docker run --name h264_mp4_encoder_temp h264_mp4_encoder /bin/true
docker cp h264_mp4_encoder_temp:/home/user/embuild embuild
docker rm h264_mp4_encoder_temp
