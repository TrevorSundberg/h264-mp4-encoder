class H264MP4Encoder {
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
}

interface FS {
    readFile(path: string): Uint8Array;
    unlink(path: string): void;
}

interface H264MP4Interface {
    then(onLoaded: (hme: H264MP4Interface) => void);
    H264MP4Encoder: typeof H264MP4Encoder;
    FS: FS;
}

export default function(): H264MP4Interface;
