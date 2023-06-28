import { wrap } from 'comlink';
import { Halo2WasmWorker, type Halo2Wasm } from '../halo2_wasm.js';
import debug from 'debug';

export async function fetchCode(name: string) {
  const res = await fetch('/' + name);
  return await res.arrayBuffer();
}

export function createWorker() {
  const worker = new Worker('halo2_wasm.js');
  const debugStr = debug.disable();
  debug.enable(debugStr);
  worker.postMessage({ debug: debugStr });
  return worker;
}

export function getRemoteBarretenbergWasm(worker: Worker): Halo2WasmWorker {
  return wrap<Halo2Wasm>(worker);
}

export function getNumCpu() {
  return navigator.hardwareConcurrency;
}

export function threadLogger(): ((msg: string) => void) | undefined {
  return undefined;
}

export function killSelf() {
  self.close();
}
