const HME = require("./embuild/dist/h264-mp4-encoder.node.js");

HME.createH264MP4Encoder().then(encoder => {
    // Must be a multiple of 2.
    encoder.width = 100;
    encoder.height = 100;
    encoder.initialize();
    // Add a single gray frame, the alpha is ignored.
    encoder.addFrameRgba(new Uint8Array(encoder.width * encoder.height * 4).fill(128))
    // For canvas:
    // encoder.addFrameRgba(ctx.getImageData(0, 0, encoder.width * encoder.height).data);
    encoder.finalize();
    const uint8Array = encoder.FS.readFile(encoder.outputFilename);
    console.log(uint8Array);
    encoder.delete();
})
