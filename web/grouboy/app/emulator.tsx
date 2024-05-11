"use client"

import React, { useEffect, useRef, useState } from 'react';
import createEmulatorModule, { MainModuleExtended } from './grouboy_wasm';

const Emulator: React.FC = () => {
    const canvasRef = useRef<HTMLCanvasElement>(null);
    const [romFile, setRomFile] = useState<File>()
    const [romLoaded, setRomLoaded] = useState(false);
    const [emulatorModule, setEmulatorModule] = useState<MainModuleExtended>();

    useEffect(() => {
        createEmulatorModule()
        .then(module => {
            console.log(`WebAssembly module loaded.`);

            // We need to set the canvas property so that SDL2 can correcly render to it.
            module.canvas = (function () {
                return canvasRef.current;
            })();

            setEmulatorModule(module);
        })
    }, []);

    const onFileSelected = (e: React.ChangeEvent<HTMLInputElement>) => {
        const files = (e.target as HTMLInputElement).files;

        if (files) {
            console.log(`setting rom file ${files[0]?.name}.`);
            setRomFile(files[0]);
        }
    };

    const startEmulatorFromRomData = (romData: ArrayBuffer) => {
        var data = new Uint8Array(romData as ArrayBuffer);
        var dataPtr = emulatorModule._malloc(data.length);
        var dataHeap = new Uint8Array(emulatorModule.HEAPU8.buffer, dataPtr, data.length);
        dataHeap.set(data);
        var success = emulatorModule._startEmulatorFromData(dataHeap.byteOffset, dataHeap.length);
        emulatorModule._free(dataHeap.byteOffset);
        setRomLoaded(success);
    };

    const onLoadRomClicked = () => {
        if (emulatorModule) {
            var reader = new FileReader();
            reader.onload = function (event: ProgressEvent<FileReader>) {
                var romData = event.target?.result as ArrayBuffer;
                if (romData) {
                    startEmulatorFromRomData(romData);
                }
            };

            reader.readAsArrayBuffer(romFile as Blob);
        }
    };

    const onLoadDefaultRomClicked = async () => {
        if (emulatorModule) {
            const request = await fetch("grouboy/tictactoe.gb");
            if (request.ok) {
                const romData = await request.arrayBuffer();
                if (romData) {
                    startEmulatorFromRomData(romData);
                }
            }
        }
    };

    return (
        <>
            {!romLoaded && (
                <>
                    <input type="file" onChange={onFileSelected} accept=".gb,.gbc" />
                    <button onClick={onLoadRomClicked}>Load your own ROM</button>
                    or  <button onClick={onLoadDefaultRomClicked}>Load default ROM</button>
                </>
            )}
            <canvas ref={canvasRef} onContextMenu={(e) => e.preventDefault()} id="canvas" />
        </>
    );
};

export default Emulator;