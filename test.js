const HME = require("./embuild/dist/h264-mp4-encoder.node.js");

HME.createH264MP4Encoder().then(encoder => {
    encoder.width = 100;
    encoder.height = 100;
    encoder.initialize();
    console.log(encoder.frameRate);
    encoder.finalize();
    encoder.delete();
})
