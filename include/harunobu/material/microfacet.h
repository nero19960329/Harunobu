#include <harunobu/material/material_base.h>

HARUNOBU_NAMESPACE_BEGIN

class Microfacet : public MaterialBase {
public:
    vec3 eta, k;
    vec3 specular_reflectance;

public:
    Microfacet() : MaterialBase() {}

    virtual vec3 f(const vec3 &wi, const vec3 &wo, const vec3 &normal) const;
    virtual void log_current_status() const;

protected:
    virtual real D(const vec3 &wh, const vec3 &normal) const = 0;
    virtual real G(const vec3 &wo, const vec3 &wi, const vec3 &normal) const;
    virtual real Lambda(const vec3 &w, const vec3 &normal) const = 0;

    vec3 fresnel(const vec3 &wo, const vec3 &wh) const;
}; // class Microfacet

class Beckmann : public Microfacet {
public:
    real alpha;

public:
    Beckmann() : Microfacet() {}

    virtual void log_current_status() const;

private:
    virtual real D(const vec3 &wh, const vec3 &normal) const override;
    virtual real Lambda(const vec3 &w, const vec3 &normal) const override;
}; // class Beckmann

HARUNOBU_NAMESPACE_END