#pragma once

#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

#include "fractal/FractalState.hpp"

using json = nlohmann::json;

namespace glm
{
	inline void to_json(json& j, const dvec2& v)
	{
		j = { v.x, v.y };
	}
	inline void from_json(const json& j, dvec2& v)
	{
		j.at(0).get_to(v.x);
		j.at(1).get_to(v.y);
	}

	inline void to_json(json& j, const vec3& v)
	{
		j = { v.x, v.y, v.z };
	}
	inline void from_json(const json& j, vec3& v)
	{
		j.at(0).get_to(v.x);
		j.at(1).get_to(v.y);
		j.at(2).get_to(v.z);
	}
}

inline void to_json(json& j, const FractalSpecificParams& p)
{
	j = { { "juliaConstant", p.juliaConstant } };
}
inline void from_json(const json& j, FractalSpecificParams& p)
{
	j.at("juliaConstant").get_to(p.juliaConstant);
}

inline void to_json(json& j, const ColorStop& cs)
{
	j = { { "color", cs.color }, { "position", cs.position } };
}
inline void from_json(const json& j, ColorStop& cs)
{
	j.at("color").get_to(cs.color);
	j.at("position").get_to(cs.position);
}

inline void to_json(json& j, const ColoringParams& p)
{
	j = { { "useSmoothing", p.useSmoothing }, { "palette", p.palette } };
}
inline void from_json(const json& j, ColoringParams& p)
{
	j.at("useSmoothing").get_to(p.useSmoothing);
	j.at("palette").get_to(p.palette);
}

inline void to_json(json& j, const FractalState& s)
{
	j = { { "renderWidth", s.renderWidth },
		  { "renderHeight", s.renderHeight },
		  { "type", s.type },
		  { "offset", s.offset },
		  { "zoom", s.zoom },
		  { "maxIterations", s.maxIterations },
		  { "specificParams", s.specificParams },
		  { "coloring", s.coloring } };
}
inline void from_json(const json& j, FractalState& s)
{
	j.at("renderWidth").get_to(s.renderWidth);
	j.at("renderHeight").get_to(s.renderHeight);
	j.at("type").get_to(s.type);
	j.at("offset").get_to(s.offset);
	j.at("zoom").get_to(s.zoom);
	j.at("maxIterations").get_to(s.maxIterations);
	j.at("specificParams").get_to(s.specificParams);
	j.at("coloring").get_to(s.coloring);
}
