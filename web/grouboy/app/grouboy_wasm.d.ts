import { MainModule } from "./grouboy_wasm_base";

// Since emscripten doesn't export properly some SDL2 property, we add them manually here.
declare namespace AdditionalExports {
  let canvas: any;
}

export type MainModuleExtended = MainModule & typeof AdditionalExports;
export default function MainModuleExtendedFactory (options?: unknown): Promise<MainModuleExtended>;
