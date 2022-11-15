#ifndef VDHit_h_
#define VDHit_h_
struct VDHit {
  float x;
  float y;
  float z;
  float time;

  float px;
  float py;
  float pz;
  float pmag;
  float ek;

  float charge;
  int   pdgId;
  unsigned particleId;
  unsigned volumeCopy;

    float totalEDep;
    float ioniEDep;
    float stepL;


VDHit() : x(std::numeric_limits<double>::quiet_NaN())
, y(std::numeric_limits<double>::quiet_NaN())
    , z(std::numeric_limits<double>::quiet_NaN())

    , time(std::numeric_limits<double>::quiet_NaN())

    , px(std::numeric_limits<double>::quiet_NaN())
    , py(std::numeric_limits<double>::quiet_NaN())
    , pz(std::numeric_limits<double>::quiet_NaN())
    , pmag(std::numeric_limits<double>::quiet_NaN())
    , ek(std::numeric_limits<double>::quiet_NaN())

    , charge(std::numeric_limits<double>::quiet_NaN())
    , pdgId(0)
    , particleId(-1U)
    , volumeCopy(-1U)
                , totalEDep(std::numeric_limits<double>::quiet_NaN())
                , ioniEDep(std::numeric_limits<double>::quiet_NaN())
                , stepL(std::numeric_limits<double>::quiet_NaN())
  {}

  /* //---------------------------------------------------------------- */
  /* VDHit( const StepPointMC& hit) */
  /* : x(hit.position().x()) */
  /* , y(hit.position().y()) */
  /*   , z(hit.position().z()) */
  /*   , time(hit.time()) */
  /*   , px(hit.momentum().x()) */
  /*   , py(hit.momentum().y()) */
  /*   , pz(hit.momentum().z()) */

  /*   , pmag(hit.momentum().mag()) */
  /*   , ek(getKineticEnergy(hit)) */

  /*   , charge(getCharge(hit.simParticle()->pdgId())) */

  /*   , pdgId(hit.simParticle()->pdgId()) */
  /*   , particleId(hit.simParticle()->id().asUint()) */
  /*   , volumeCopyNumber(hit.volumeId()) */
  /* {} */

}; // struct VDHit
#endif
