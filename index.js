const H264MP4Module = require("./commonjs");

H264MP4Module().then((hme) => {
    const encoder = new hme.H264MP4Encoder();
    encoder.width = 100;
    encoder.height = 100;
    encoder.initialize();
    console.log(encoder);
})