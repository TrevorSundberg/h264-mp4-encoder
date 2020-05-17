import Module from "./embuild/h264-mp4-encoder.js";
const hme = (Module as any)();
const promise = new Promise(resolve => {
    hme.then(() => {
        resolve();
    })
})

// Type definitions for Emscripten 1.39.16
// Project: https://emscripten.org
// Definitions by: Kensuke Matsuzaki <https://github.com/zakki>
//                 Periklis Tsirakidis <https://github.com/periklis>
//                 Bumsik Kim <https://github.com/kbumsik>
//                 Louis DeScioli <https://github.com/lourd>
// Definitions: https://github.com/DefinitelyTyped/DefinitelyTyped
// TypeScript Version: 2.2
export declare namespace FS {
    interface Lookup {
        path: string;
        node: FSNode;
    }

    interface FSStream { }
    interface FSNode { }
    interface ErrnoError { }

    let ignorePermissions: boolean;
    let trackingDelegate: any;
    let tracking: any;
    let genericErrors: any;

    //
    // paths
    //
    function lookupPath(path: string, opts: any): Lookup;
    function getPath(node: FSNode): string;

    //
    // nodes
    //
    function isFile(mode: number): boolean;
    function isDir(mode: number): boolean;
    function isLink(mode: number): boolean;
    function isChrdev(mode: number): boolean;
    function isBlkdev(mode: number): boolean;
    function isFIFO(mode: number): boolean;
    function isSocket(mode: number): boolean;

    //
    // devices
    //
    function major(dev: number): number;
    function minor(dev: number): number;
    function makedev(ma: number, mi: number): number;
    function registerDevice(dev: number, ops: any): void;

    //
    // core
    //
    function syncfs(populate: boolean, callback: (e: any) => any): void;
    function syncfs(callback: (e: any) => any, populate?: boolean): void;
    function mount(type: any, opts: any, mountpoint: string): any;
    function unmount(mountpoint: string): void;

    function mkdir(path: string, mode?: number): any;
    function mkdev(path: string, mode?: number, dev?: number): any;
    function symlink(oldpath: string, newpath: string): any;
    function rename(old_path: string, new_path: string): void;
    function rmdir(path: string): void;
    function readdir(path: string): any;
    function unlink(path: string): void;
    function readlink(path: string): string;
    function stat(path: string, dontFollow?: boolean): any;
    function lstat(path: string): any;
    function chmod(path: string, mode: number, dontFollow?: boolean): void;
    function lchmod(path: string, mode: number): void;
    function fchmod(fd: number, mode: number): void;
    function chown(path: string, uid: number, gid: number, dontFollow?: boolean): void;
    function lchown(path: string, uid: number, gid: number): void;
    function fchown(fd: number, uid: number, gid: number): void;
    function truncate(path: string, len: number): void;
    function ftruncate(fd: number, len: number): void;
    function utime(path: string, atime: number, mtime: number): void;
    function open(path: string, flags: string, mode?: number, fd_start?: number, fd_end?: number): FSStream;
    function close(stream: FSStream): void;
    function llseek(stream: FSStream, offset: number, whence: number): any;
    function read(stream: FSStream, buffer: ArrayBufferView, offset: number, length: number, position?: number): number;
    function write(
        stream: FSStream,
        buffer: ArrayBufferView,
        offset: number,
        length: number,
        position?: number,
        canOwn?: boolean,
    ): number;
    function allocate(stream: FSStream, offset: number, length: number): void;
    function mmap(
        stream: FSStream,
        buffer: ArrayBufferView,
        offset: number,
        length: number,
        position: number,
        prot: number,
        flags: number,
    ): any;
    function ioctl(stream: FSStream, cmd: any, arg: any): any;
    function readFile(path: string, opts: { encoding: 'binary'; flags?: string }): Uint8Array;
    function readFile(path: string, opts: { encoding: 'utf8'; flags?: string }): string;
    function readFile(path: string, opts?: { flags?: string }): Uint8Array;
    function writeFile(path: string, data: string | ArrayBufferView, opts?: { flags?: string }): void;

    //
    // module-level FS code
    //
    function cwd(): string;
    function chdir(path: string): void;
    function init(
        input: null | (() => number | null),
        output: null | ((c: number) => any),
        error: null | ((c: number) => any),
    ): void;

    function createLazyFile(
        parent: string | FSNode,
        name: string,
        url: string,
        canRead: boolean,
        canWrite: boolean,
    ): FSNode;
    function createPreloadedFile(
        parent: string | FSNode,
        name: string,
        url: string,
        canRead: boolean,
        canWrite: boolean,
        onload?: () => void,
        onerror?: () => void,
        dontCreateFile?: boolean,
        canOwn?: boolean,
    ): void;
    function createDataFile(
        parent: string | FSNode,
        name: string,
        data: ArrayBufferView,
        canRead: boolean,
        canWrite: boolean,
        canOwn: boolean,
    ): FSNode;
}

export interface H264MP4Encoder {
    /**
     * Name of the file that we output.
     * After `encoder.finalize()` use `encoder.FS.readFile(encoder.outputFilename)` after `finalize`.
     * @default "output.mp4"
     */
    outputFilename: string;

    /**
     * Width of the input frames and output video. Must be a multiple of 2. Required.
     * @default 0
     */
    width: number;

    /**
     * Height of the input frames and output video. Must be a multiple of 2. Required.
     * @default 0
     */
    height: number;

    /**
     * Frame rate of the output video.
     * @default 30
     */
    frameRate: number;

    /**
     * The bitrate in kbps relative to the frame_rate. Overwrites quantization_parameter if not 0.
     * @default 0
     */
    kbps: number;

    /**
     * Speed where 0 means best quality and 10 means fastest speed [0..10].
     * @default 0
     */
    speed: number;

    /**
     * Higher means better compression, and lower means better quality [10..51].
     * @default 33
     */
    quantizationParameter: number;

    /**
     * How often a keyframe occurs (key frame period, also known as GOP).
     * @default 20
     */
    groupOfPictures: number;

    /**
     * Use temporal noise supression.
     * @default false
     */
    temporalDenoise: boolean;

    /**
     * Each NAL unit will be approximately capped at this size (0 means unlimited).
     * @default 0
     */
    desiredNaluBytes: number;

    /**
     * Prints extra debug information.
     * @default false
     */
    debug: boolean;

    /** Initialize the encoder. After calling this, all parameters above will be readonly. */
    initialize(): void;

    /** Add a frame in YUV format (expected size: width * height * 3 / 2). */
    addFrameYuv(buffer: ArrayBuffer | Uint8Array | Uint8ClampedArray | Int8Array | string): void;

    /**
     * Add a frame in RGBA format (expected size: width * height * 4).
     * Alpha is ignored but is kept for convenience of working with the HTML5 canvas.getImageData().
     */
    addFrameRgba(buffer: ArrayBuffer | Uint8Array | Uint8ClampedArray | Int8Array | string): void;

    /** Finish outputting the video to the `outputFilename`. */
    finalize(): void;

    /** Delete this object and free all resources. Should not be used again. */
    delete(): void;

    /**
     * A reference to Emscripten's virtual file system.
     * The output files will be written here under the provided `outputFilename`.
     * This file system is *shared* between all H264MP4Encoders.
     */
    FS: typeof FS;
}

/** Construct the H264MP4Encoder. Waits for the WASM to complete loading before returning. */
export const createH264MP4Encoder = async (): Promise<H264MP4Encoder> => {
    await promise;
    const encoder: H264MP4Encoder = new hme.H264MP4Encoder();
    encoder.FS = hme.FS;
    return encoder;
}
