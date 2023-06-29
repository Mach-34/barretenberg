import { Crs } from '../index.js';
import { Halo2ApiAsync, newHalo2ApiAsync } from '../factory/index.js';
import { RawBuffer } from '../types/index.js';

describe('simple', () => {
  let api: Halo2ApiAsync;

  beforeAll(async () => {
    api = await newHalo2ApiAsync();

    // Important to init slab allocator as first thing, to ensure maximum memory efficiency.
    const CIRCUIT_SIZE = 2 ** 19;
    await api.commonInitSlabAllocator(CIRCUIT_SIZE);

    const crs = await Crs.new(2 ** 19 + 1);
    await api.srsInitSrs(new RawBuffer(crs.getG1Data()), crs.numPoints, new RawBuffer(crs.getG2Data()));
  }, 20000);

  afterAll(async () => {
    await api.destroy();
  });

  it('should construct 512k gate proof', async () => {
    const valid = await api.examplesSimpleCreateAndVerifyProof();
    expect(valid).toBe(true);
  }, 90000);
});
