docker build -t h264_mp4_encoder .
rm -rf build_emscripten
docker run --name h264_mp4_encoder_temp h264_mp4_encoder /bin/true
docker cp h264_mp4_encoder_temp:/home/user/embuild build_emscripten
docker rm h264_mp4_encoder_temp
