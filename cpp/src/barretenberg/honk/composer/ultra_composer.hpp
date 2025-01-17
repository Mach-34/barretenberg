#pragma once

#include "barretenberg/proof_system/composer/composer_lib.hpp"
#include "barretenberg/srs/factories/file_crs_factory.hpp"
#include "barretenberg/honk/proof_system/ultra_prover.hpp"
#include "barretenberg/honk/proof_system/ultra_verifier.hpp"

#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

namespace proof_system::honk {
template <UltraFlavor Flavor> class UltraComposer_ {
  public:
    using CircuitBuilder = typename Flavor::CircuitBuilder;
    using ProvingKey = typename Flavor::ProvingKey;
    using VerificationKey = typename Flavor::VerificationKey;
    using PCSParams = typename Flavor::PCSParams;
    using PCS = typename Flavor::PCS;
    using PCSCommitmentKey = typename PCSParams::CommitmentKey;
    using PCSVerificationKey = typename PCSParams::VerificationKey;

    static constexpr std::string_view NAME_STRING = "UltraHonk";
    static constexpr size_t NUM_RESERVED_GATES = 4; // equal to the number of multilinear evaluations leaked
    static constexpr size_t NUM_WIRES = CircuitBuilder::NUM_WIRES;
    std::shared_ptr<ProvingKey> proving_key;
    std::shared_ptr<VerificationKey> verification_key;

    // The crs_factory holds the path to the srs and exposes methods to extract the srs elements
    std::shared_ptr<srs::factories::CrsFactory> crs_factory_;

    // The commitment key is passed to the prover but also used herein to compute the verfication key commitments
    std::shared_ptr<PCSCommitmentKey> commitment_key;

    std::vector<uint32_t> recursive_proof_public_input_indices;
    bool contains_recursive_proof = false;
    bool computed_witness = false;

    UltraComposer_()
        : crs_factory_(barretenberg::srs::get_crs_factory()){};

    explicit UltraComposer_(std::shared_ptr<srs::factories::CrsFactory> crs_factory)
        : crs_factory_(std::move(crs_factory))
    {}

    UltraComposer_(std::shared_ptr<ProvingKey> p_key, std::shared_ptr<VerificationKey> v_key)
        : proving_key(std::move(p_key))
        , verification_key(std::move(v_key))
    {}

    UltraComposer_(UltraComposer_&& other) noexcept = default;
    UltraComposer_(UltraComposer_ const& other) noexcept = default;
    UltraComposer_& operator=(UltraComposer_&& other) noexcept = default;
    UltraComposer_& operator=(UltraComposer_ const& other) noexcept = default;
    ~UltraComposer_() = default;

    std::shared_ptr<ProvingKey> compute_proving_key(const CircuitBuilder& circuit_constructor);
    std::shared_ptr<VerificationKey> compute_verification_key(const CircuitBuilder& circuit_constructor);

    void compute_witness(CircuitBuilder& circuit_constructor);

    UltraProver_<Flavor> create_prover(CircuitBuilder& circuit_constructor);
    UltraVerifier_<Flavor> create_verifier(const CircuitBuilder& circuit_constructor);

    void add_table_column_selector_poly_to_proving_key(polynomial& small, const std::string& tag);

    void compute_commitment_key(size_t circuit_size, std::shared_ptr<srs::factories::CrsFactory> crs_factory)
    {
        commitment_key = std::make_shared<typename PCSParams::CommitmentKey>(circuit_size, crs_factory_);
    };
};
extern template class UltraComposer_<honk::flavor::Ultra>;
extern template class UltraComposer_<honk::flavor::UltraGrumpkin>;
// TODO(#532): this pattern is weird; is this not instantiating the templates?
using UltraComposer = UltraComposer_<honk::flavor::Ultra>;
using UltraGrumpkinComposer = UltraComposer_<honk::flavor::UltraGrumpkin>;
} // namespace proof_system::honk
