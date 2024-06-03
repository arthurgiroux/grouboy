"use client"

import React, { useEffect, useRef, useState } from 'react';
import createEmulatorModule, { MainModule } from './grouboy_wasm';

const Emulator: React.FC = () => {
    const canvasRef = useRef<HTMLCanvasElement>(null);
    const [romFile, setRomFile] = useState<File>()
    const [romLoaded, setRomLoaded] = useState(false);
    const [emulatorModule, setEmulatorModule] = useState<MainModule>();
    const [output, setOutput] = useState<string>("");
    const [emulatorPtr, setEmulatorPtr] = useState<number>(0);

    useEffect(() => {
        const cleanup = () => {
            if (emulatorModule) {
                emulatorModule._destroy(emulatorPtr);
            }
        };

        createEmulatorModule({
            print: (text: string) => {
                console.log(text);
                setOutput((prevOutput) => prevOutput + text);
            },
            printErr: (text: string) => {
                console.error(text);
                setOutput((prevOutput) => prevOutput + text);
            },
            canvas: canvasRef.current
        })
        .then(module => {
            console.log(`WebAssembly module loaded.`);
            setEmulatorModule(module);
            setEmulatorPtr(module._init());
        });

        return cleanup;
    }, []);

    const onFileSelected = (e: React.ChangeEvent<HTMLInputElement>) => {
        const files = (e.target as HTMLInputElement).files;

        if (files) {
            console.log(`setting rom file ${files[0]?.name}.`);
            setRomFile(files[0]);
        }
    };

    const startEmulatorFromRomData = (romData: ArrayBuffer) => {
        if (emulatorModule) {
            var data = new Uint8Array(romData as ArrayBuffer);
            var dataPtr = emulatorModule._malloc(data.length);
            var dataHeap = new Uint8Array(emulatorModule.HEAPU8.buffer, dataPtr, data.length);
            dataHeap.set(data);
            var success = emulatorModule._loadROM(emulatorPtr, dataHeap.byteOffset, dataHeap.length);
            emulatorModule._free(dataHeap.byteOffset);
            setRomLoaded(success === 1);
            if (success) {
                emulatorModule._start(emulatorPtr);
            }
        }
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
            <>{output}</>
        </>
    );
};

export default Emulator;