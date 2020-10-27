# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]
### Added
- Integrator
  - Support path tracing
- Sampler
  - Support stratified sampler
### Changed
- Framework
  - Seperate openmp in debug & release mode
- Others
  - Reconstruct sampler & integrator
### Fixed
- Sampler
  - Fix sampling on two-sided material
- Reconstruction filter
  - Fix pixel sample accumulating bug

## [0.0.1] - 2020-10-27
### Added
- Camera
  - Support perspective camera
- Image pipeline
  - Support gamma correction
- Geometry & Primitive
  - Support triangle/sphere
  - Support cube/rectangle
- Material
  - Support diffuse/microface(Beckmann)
- IO
  - Support mitsuba-styled scene file
    - Not completely supported
- Integrator
  - Support direct illumination
- Reconstruction filter
  - Support box/tent filter
- Sampler
  - Support random sampler
- Framework
  - Support bazel compiling

[Unreleased]: https://github.com/nero19960329/Harunobu/tree/framework-v2
[0.0.1]: https://github.com/nero19960329/Harunobu/tree/v0.0.1