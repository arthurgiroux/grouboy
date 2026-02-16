"use client"

import React, { useCallback, useEffect, useRef, useState } from 'react';
import createEmulatorModule, { MainModule } from './grouboy_wasm';

const GB_WIDTH = 160;
const GB_HEIGHT = 144;
const SCALE = 4;

function GamepadIcon() {
    return (
        <svg viewBox="0 0 24 24" width="48" height="48" fill="none" stroke="currentColor" strokeWidth="1.5" strokeLinecap="round" strokeLinejoin="round" className="text-gb-light/40">
            <path d="M6 11h4M8 9v4" />
            <line x1="15" y1="12" x2="15.01" y2="12" />
            <line x1="18" y1="10" x2="18.01" y2="10" />
            <path d="M17.32 5H6.68a4 4 0 00-3.978 3.59c-.006.052-.01.101-.017.152C2.604 9.416 2 14.456 2 16a3 3 0 003 3c1 0 1.5-.5 2-1l1.414-1.414A2 2 0 019.828 16h4.344a2 2 0 011.414.586L17 18c.5.5 1 1 2 1a3 3 0 003-3c0-1.545-.604-6.584-.685-7.258-.007-.05-.011-.1-.017-.151A4 4 0 0017.32 5z" />
        </svg>
    );
}

const Emulator: React.FC = () => {
    const canvasRef = useRef<HTMLCanvasElement>(null);
    const fileInputRef = useRef<HTMLInputElement>(null);
    const emulatorModuleRef = useRef<MainModule | null>(null);
    const emulatorPtrRef = useRef<number>(0);
    const [romName, setRomName] = useState<string>("");
    const [romLoaded, setRomLoaded] = useState(false);
    const [emulatorModule, setEmulatorModule] = useState<MainModule>();
    const [isDragOver, setIsDragOver] = useState(false);
    const [isLoading, setIsLoading] = useState(true);

    useEffect(() => {
        createEmulatorModule({
            print: (text: string) => {
                console.log(text);
            },
            printErr: (text: string) => {
                console.error(text);
            },
            canvas: canvasRef.current
        })
        .then(module => {
            console.log('WebAssembly module loaded.');
            emulatorModuleRef.current = module;
            emulatorPtrRef.current = module._init();
            setEmulatorModule(module);
            setIsLoading(false);
        });

        return () => {
            if (emulatorModuleRef.current && emulatorPtrRef.current) {
                emulatorModuleRef.current._destroy(emulatorPtrRef.current);
            }
        };
    }, []);

    const startEmulatorFromRomData = useCallback((romData: ArrayBuffer) => {
        if (emulatorModule && emulatorPtrRef.current) {
            const data = new Uint8Array(romData);
            const dataPtr = emulatorModule._malloc(data.length);
            const dataHeap = new Uint8Array(emulatorModule.HEAPU8.buffer, dataPtr, data.length);
            dataHeap.set(data);
            const success = emulatorModule._loadROM(emulatorPtrRef.current, dataHeap.byteOffset, dataHeap.length);
            emulatorModule._free(dataHeap.byteOffset);
            setRomLoaded(success === 1);
            if (success) {
                emulatorModule._start(emulatorPtrRef.current);
            }
        }
    }, [emulatorModule]);

    const loadFileAsRom = useCallback((file: File) => {
        const reader = new FileReader();
        reader.onload = (event) => {
            const romData = event.target?.result as ArrayBuffer;
            if (romData) {
                startEmulatorFromRomData(romData);
            }
        };
        reader.readAsArrayBuffer(file);
    }, [startEmulatorFromRomData]);

    const onFileSelected = useCallback((e: React.ChangeEvent<HTMLInputElement>) => {
        const files = e.target.files;
        if (files && files[0]) {
            setRomName(files[0].name);
            loadFileAsRom(files[0]);
        }
    }, [loadFileAsRom]);

    const onLoadDefaultRomClicked = async () => {
        if (emulatorModule) {
            const request = await fetch("grouboy/tictactoe.gb");
            if (request.ok) {
                const romData = await request.arrayBuffer();
                if (romData) {
                    setRomName("tictactoe.gb");
                    startEmulatorFromRomData(romData);
                }
            }
        }
    };

    const onDragOver = useCallback((e: React.DragEvent) => {
        e.preventDefault();
        e.stopPropagation();
        setIsDragOver(true);
    }, []);

    const onDragLeave = useCallback((e: React.DragEvent) => {
        e.preventDefault();
        e.stopPropagation();
        setIsDragOver(false);
    }, []);

    const onDrop = useCallback((e: React.DragEvent) => {
        e.preventDefault();
        e.stopPropagation();
        setIsDragOver(false);

        const files = e.dataTransfer.files;
        if (files && files[0]) {
            const file = files[0];
            if (file.name.endsWith('.gb') || file.name.endsWith('.gbc')) {
                setRomName(file.name);
                loadFileAsRom(file);
            }
        }
    }, [loadFileAsRom]);

    return (
        <div className="w-full max-w-2xl">
            {/* Screen bezel container */}
            <div className="screen-bezel rounded-3xl p-5 sm:p-8">
                {/* Power LED indicator */}
                <div className="flex items-center gap-2 mb-4">
                    <div className={`w-2 h-2 rounded-full transition-colors duration-500 ${
                        romLoaded ? 'bg-gb-lightest shadow-[0_0_6px_rgba(155,188,15,0.6)]' : 'bg-slate-700'
                    }`} />
                    <span className="text-[10px] tracking-widest uppercase text-slate-600">
                        {romLoaded ? 'Running' : isLoading ? 'Initializing...' : 'Ready'}
                    </span>
                </div>

                {/* Screen area */}
                <div className="screen-inner rounded-xl overflow-hidden relative">
                    {/* Canvas (always rendered, hidden behind loader when no ROM) */}
                    <canvas
                        ref={canvasRef}
                        onContextMenu={(e) => e.preventDefault()}
                        id="canvas"
                        width={GB_WIDTH}
                        height={GB_HEIGHT}
                        className="w-full block"
                        style={{
                            aspectRatio: `${GB_WIDTH} / ${GB_HEIGHT}`,
                            maxWidth: `${GB_WIDTH * SCALE}px`,
                        }}
                    />

                    {/* ROM loading overlay */}
                    {!romLoaded && (
                        <div className="absolute inset-0 flex flex-col items-center justify-center bg-surface-900/95 backdrop-blur-sm">
                            {isLoading ? (
                                <div className="flex flex-col items-center gap-3">
                                    <div className="w-6 h-6 border-2 border-gb-light/30 border-t-gb-lightest rounded-full animate-spin" />
                                    <span className="text-slate-500 text-xs">Loading WebAssembly...</span>
                                </div>
                            ) : (
                                <div
                                    className={`drop-zone w-[90%] max-w-md p-8 flex flex-col items-center gap-5 cursor-pointer ${
                                        isDragOver ? 'drag-over' : ''
                                    }`}
                                    onDragOver={onDragOver}
                                    onDragLeave={onDragLeave}
                                    onDrop={onDrop}
                                    onClick={() => fileInputRef.current?.click()}
                                >
                                    <GamepadIcon />

                                    <div className="text-center">
                                        <p className="text-slate-300 text-sm mb-1">
                                            Drop a ROM file here
                                        </p>
                                        <p className="text-slate-600 text-xs">
                                            or click to browse &middot; .gb / .gbc
                                        </p>
                                    </div>

                                    <input
                                        ref={fileInputRef}
                                        type="file"
                                        onChange={onFileSelected}
                                        accept=".gb,.gbc"
                                        className="hidden"
                                    />

                                    <div className="flex items-center gap-3 w-full">
                                        <div className="flex-1 h-px bg-white/5" />
                                        <span className="text-slate-600 text-xs">or</span>
                                        <div className="flex-1 h-px bg-white/5" />
                                    </div>

                                    <button
                                        onClick={(e) => {
                                            e.stopPropagation();
                                            onLoadDefaultRomClicked();
                                        }}
                                        className="btn-primary w-full text-center"
                                    >
                                        Try Demo ROM
                                    </button>
                                </div>
                            )}
                        </div>
                    )}
                </div>

                {/* Bottom bar */}
                <div className="flex items-center justify-between mt-4">
                    <div className="flex items-center gap-2">
                        {romLoaded && (
                            <>
                                <div className="w-1.5 h-1.5 rounded-full bg-gb-light/60" />
                                <span className="text-slate-500 text-xs truncate max-w-[200px]">
                                    {romName}
                                </span>
                            </>
                        )}
                    </div>
                    {romLoaded && (
                        <button
                            onClick={() => window.location.reload()}
                            className="text-slate-600 text-xs hover:text-slate-400 transition-colors"
                        >
                            Load another
                        </button>
                    )}
                </div>
            </div>
        </div>
    );
};

export default Emulator;
