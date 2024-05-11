"use client"

import React, { useRef, useState } from 'react';
import createEmulatorModule from './grouboy_wasm';

const Emulator: React.FC = () => {
    const canvasRef = useRef<HTMLCanvasElement>(null);
    const [romFile, setRomFile] = useState<File>()
    const [romLoaded, setRomLoaded] = useState(false);

    const onFileSelected = (e: React.ChangeEvent<HTMLInputElement>) => {
        const files = (e.target as HTMLInputElement).files;

        if (files) {
            console.log(`setting rom file ${files[0].name}.`);
            setRomFile(files[0]);
        }
    };

    const onLoadRomClicked = () => {
        createEmulatorModule()
            .then(module => {
                console.log(`WebAssembly module loaded.`);

                // We need to set the canvas property so that SDL2 can correcly render to it.
                module.canvas = (function () {
                    return canvasRef.current;
                })();

                var reader = new FileReader();
                reader.onload = function (event: ProgressEvent<FileReader>) {
                    var romData = event.target?.result;
                    if (romData) {
                        var data = new Uint8Array(romData as ArrayBuffer);
                        var dataPtr = module._malloc(data.length);
                        var dataHeap = new Uint8Array(module.HEAPU8.buffer, dataPtr, data.length);
                        dataHeap.set(data);
                        module._startEmulatorFromData(dataHeap.byteOffset, dataHeap.length);
                        module._free(dataHeap.byteOffset);
                        setRomLoaded(true);
                    }
                };

                reader.readAsArrayBuffer(romFile as Blob);
            }
        )
    };

    return (
        <>
            {!romLoaded && (
                <>
                    <input type="file" onChange={onFileSelected} accept=".gb,.gbc" />
                    <button onClick={onLoadRomClicked}>Load your own ROM</button>
                </>
            )}
            <canvas ref={canvasRef} onContextMenu={(e) => e.preventDefault()} id="canvas" />
        </>
    );
};

export default Emulator;