import { expose } from 'comlink';
import { Halo2Wasm } from '../index.js';
import debug from 'debug';

self.onmessage = function (e) {
  if (e.data.debug) {
    debug.enable(e.data.debug);
  }
};

expose(new Halo2Wasm());

self.postMessage({ ready: true });
