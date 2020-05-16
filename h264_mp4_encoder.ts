const Module = require("./embuild/h264_mp4_encoder.js");
const hme = Module();
const promise = new Promise(resolve => {
    hme.then(() => {
        resolve();
    })
})

declare class H264MP4Encoder {
    outputFilename: number;
    width: number;
    height: number;
    frameRate: number;
    kbps: number;
    speed: number;
    quantizationParameter: number;
    groupOfPictures: number;
    temporalDenoise: number;
    desiredNaluBytes: number;
    debug: number;

    initialize(): void;
    addFrameYuv(buffer: ArrayBuffer |  Uint8Array | Uint8ClampedArray | Int8Array | string): void;
    addFrameRgba(buffer: ArrayBuffer |  Uint8Array | Uint8ClampedArray | Int8Array | string): void;
    finalize(): void;
    delete(): void;

    FS: typeof FS;
}

export const createH264MP4Encoder = async() : Promise<H264MP4Encoder> => {
    await promise;
    const encoder: H264MP4Encoder = new hme.H264MP4Encoder();
    encoder.FS = hme.FS;
    return encoder;
}
