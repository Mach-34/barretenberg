#include <cstdint>
#include <cstddef>
#include "barretenberg/ecc/curves/bn254/fr.hpp"
#include "barretenberg/common/wasm_export.hpp"

extern "C" {

using namespace barretenberg;

WASM_EXPORT void blake2s(uint8_t const* data, out_buf32 r);

WASM_EXPORT void blake2s_to_field_(uint8_t const* data, fr::out_buf r);
}
