#pragma once

#include <utility>
#include "barretenberg/srs/factories/file_crs_factory.hpp"
#include "barretenberg/honk/proof_system/prover.hpp"
#include "barretenberg/honk/proof_system/verifier.hpp"
#include "barretenberg/proof_system/circuit_builder/standard_circuit_builder.hpp"
#include "barretenberg/proof_system/composer/composer_lib.hpp"
#include "barretenberg/proof_system/composer/permutation_lib.hpp"

#include "barretenberg/honk/flavor/standard.hpp"
#include "barretenberg/honk/flavor/standard_grumpkin.hpp"

namespace proof_system::honk {
template <StandardFlavor Flavor> class StandardComposer_ {
  public:
    using PCSParams = typename Flavor::PCSParams;
    using CircuitBuilder = typename Flavor::CircuitBuilder;
    using ProvingKey = typename Flavor::ProvingKey;
    using VerificationKey = typename Flavor::VerificationKey;
    using PCSCommitmentKey = typename PCSParams::CommitmentKey;

    static constexpr std::string_view NAME_STRING = "StandardHonk";
    static constexpr size_t NUM_RESERVED_GATES = 2; // equal to the number of multilinear evaluations leaked
    static constexpr size_t NUM_WIRES = CircuitBuilder::NUM_WIRES;
    std::shared_ptr<ProvingKey> proving_key;
    std::shared_ptr<VerificationKey> verification_key;

    // The crs_factory holds the path to the srs and exposes methods to extract the srs elements
    std::shared_ptr<srs::factories::CrsFactory> crs_factory_;

    // The commitment key is passed to the prover but also used herein to compute the verfication key commitments
    std::shared_ptr<PCSCommitmentKey> commitment_key;

    bool computed_witness = false;
    // TODO(Luke): use make_shared
    StandardComposer_()
        : StandardComposer_(
              std::shared_ptr<srs::factories::CrsFactory>(new srs::factories::FileCrsFactory("../srs_db/ignition")))
    {}
    StandardComposer_(std::shared_ptr<srs::factories::CrsFactory> crs_factory)
        : crs_factory_(std::move(crs_factory))
    {}

    StandardComposer_(std::unique_ptr<srs::factories::CrsFactory>&& crs_factory)
        : crs_factory_(std::move(crs_factory))
    {}
    StandardComposer_(std::shared_ptr<ProvingKey> p_key, std::shared_ptr<VerificationKey> v_key)
        : proving_key(std::move(p_key))
        , verification_key(std::move(v_key))
    {}
    StandardComposer_(StandardComposer_&& other) noexcept = default;
    StandardComposer_(const StandardComposer_& other) = delete;
    StandardComposer_& operator=(StandardComposer_&& other) noexcept = default;
    StandardComposer_& operator=(const StandardComposer_& other) = delete;
    ~StandardComposer_() = default;

    std::shared_ptr<ProvingKey> compute_proving_key(const CircuitBuilder& circuit_constructor);
    std::shared_ptr<VerificationKey> compute_verification_key(const CircuitBuilder& circuit_constructor);

    StandardVerifier_<Flavor> create_verifier(const CircuitBuilder& circuit_constructor);

    StandardProver_<Flavor> create_prover(const CircuitBuilder& circuit_constructor);

    // TODO(#216)(Adrian): Seems error prone to provide the number of randomized gates
    std::shared_ptr<ProvingKey> compute_proving_key_base(const CircuitBuilder& circuit_constructor,
                                                         const size_t minimum_circuit_size = 0,
                                                         const size_t num_randomized_gates = NUM_RESERVED_GATES);

    void compute_witness(const CircuitBuilder& circuit_constructor, const size_t minimum_circuit_size = 0);

    void compute_commitment_key(size_t circuit_size, std::shared_ptr<srs::factories::CrsFactory> crs_factory)
    {
        commitment_key = std::make_shared<typename PCSParams::CommitmentKey>(circuit_size, crs_factory_);
    };
};

extern template class StandardComposer_<honk::flavor::Standard>;
extern template class StandardComposer_<honk::flavor::StandardGrumpkin>;
// TODO(#532): this pattern is weird; is this not instantiating the templates?
using StandardComposer = StandardComposer_<honk::flavor::Standard>;
using StandardGrumpkinComposer = StandardComposer_<honk::flavor::StandardGrumpkin>;
} // namespace proof_system::honk
