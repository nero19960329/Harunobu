#include <harunobu/material/material_base.h>

HARUNOBU_NAMESPACE_BEGIN

class Microfacet : public MaterialBase {
public:
    vec3 eta, k;
    vec3 specular_reflectance;

public:
    Microfacet() : MaterialBase() {}

    virtual vec3 f(sptr<LocalInfo> linfo) const;
    virtual void sample(sptr<LocalInfo> linfo) const;
    virtual real pdf(sptr<LocalInfo> linfo) const;
    virtual void log_current_status() const;

protected:
    virtual vec3 sample_wh(sptr<LocalInfo> linfo) const = 0;

    virtual real D(const vec3 &wh, sptr<LocalInfo> linfo) const = 0;
    virtual real G(sptr<LocalInfo> linfo) const;
    virtual real G1(const vec3 &w, sptr<LocalInfo> linfo) const;
    virtual real Lambda(const vec3 &w, sptr<LocalInfo> linfo) const = 0;

    vec3 fresnel(const vec3 &wo, const vec3 &wh) const;
}; // class Microfacet

class Beckmann : public Microfacet {
public:
    real alpha;

public:
    Beckmann() : Microfacet() {}

    virtual void log_current_status() const;

private:
    virtual vec3 sample_wh(sptr<LocalInfo> linfo) const override;

    virtual real D(const vec3 &wh, sptr<LocalInfo> linfo) const override;
    virtual real Lambda(const vec3 &w, sptr<LocalInfo> linfo) const override;
}; // class Beckmann

HARUNOBU_NAMESPACE_END