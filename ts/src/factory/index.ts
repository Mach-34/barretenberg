import { Halo2Api, Halo2ApiSync } from '../halo2_api/index.js';
import { Halo2Binder, Halo2BinderSync } from '../halo2_binder/index.js';
import { Halo2Wasm, Halo2WasmWorker } from '../halo2_wasm/index.js';

/**
 * Returns a single threaded, synchronous, halo2 api.
 * Can be used on the main thread to perform small light-weight requests like hashing etc.
 */
export async function newHalo2ApiSync() {
  return new Halo2ApiSync(new Halo2BinderSync(await Halo2Wasm.new()));
}

export class Halo2ApiAsync extends Halo2Api {
  constructor(private worker: any, private wasm: Halo2WasmWorker) {
    super(new Halo2Binder(wasm));
  }

  async getNumThreads() {
    return await this.wasm.getNumThreads();
  }

  async destroy() {
    await this.wasm.destroy();
    await this.worker.terminate();
  }
}

/**
 * Returns a multi threaded, asynchronous, halo2 api.
 * It runs in a worker, and so can be used within the browser to execute long running, multi-threaded requests
 * like proof construction etc.
 */
export async function newHalo2ApiAsync(threads?: number) {
  const { wasm, worker } = await Halo2Wasm.newWorker(threads);
  return new Halo2ApiAsync(worker, wasm);
}
