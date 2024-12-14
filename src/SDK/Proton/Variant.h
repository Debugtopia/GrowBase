#pragma once
#include <string>
#include <SDK/Proton/Math.h>
#define C_MAX_VARIANT_LIST_PARMS 7

class Variant
{
public:
    enum eType
	{
		TYPE_UNUSED,
		TYPE_FLOAT,
		TYPE_STRING,
		TYPE_VECTOR2,
		TYPE_VECTOR3,
		TYPE_UINT32,
		TYPE_ENTITY,
		TYPE_COMPONENT,
		TYPE_RECT,
		TYPE_INT32
	};

	Variant(uint32_t var) { Set(var); }
	Variant(int32_t var) { Set(var); }
	Variant(float var) { Set(var); }
	Variant(const std::string& var) { Set(var); }
    Variant(const char* var) { Set(std::string(var)); }
	Variant(float x, float y) { Set(CL_Vec2f(x, y)); }
	Variant(float x, float y, float z) { Set(CL_Vec3f(x, y, z)); }
	Variant(const CL_Vec2f& v2) { Set(v2); }
	Variant(const CL_Vec3f& v3) { Set(v3); }

    Variant();
    ~Variant();

public:
	eType GetType() const { return m_type; }

    void Set(float var);
	float& GetFloat();
	const float& GetFloat() const;

    void Set(uint32_t var);
	uint32_t& GetUInt32();
	const uint32_t& GetUInt32() const;

    void Set(int32_t var);
	int32_t& GetInt32();
	const int32_t& GetInt32() const;

    void Set(std::string const &var);
	std::string& GetString();
	const std::string& GetString() const;

	void Set(float x, float y) { Set(CL_Vec2f(x, y)); }
    void Set(const CL_Vec2f& var);
	CL_Vec2f& GetVector2();
	const CL_Vec2f& GetVector2() const;

	void Set(float x, float y, float z) { Set(CL_Vec3f(x, y ,z)); }
	void Set(const CL_Vec3f& var);
	CL_Vec3f& GetVector3();
	const CL_Vec3f& GetVector3() const;

	friend class VariantList;

private:
    eType m_type;
	void* m_pVoid;
	uint8_t m_var[12]; // (int * 3) // add more 4 for Rect
	std::string m_string;
};

class VariantList
{
public:
	VariantList() = default;
    VariantList(Variant v0) { m_variant[0] = v0; }
    VariantList(Variant v0, Variant v1) { m_variant[0] = v0; m_variant[1] = v1; }
    VariantList(Variant v0, Variant v1, Variant v2) { m_variant[0] = v0; m_variant[1] = v1; m_variant[2] = v2; }
    VariantList(Variant v0, Variant v1, Variant v2, Variant v3) { m_variant[0] = v0; m_variant[1] = v1; m_variant[2] = v2; m_variant[3] = v3; }
    VariantList(Variant v0, Variant v1, Variant v2, Variant v3, Variant v4) { m_variant[0] = v0; m_variant[1] = v1; m_variant[2] = v2; m_variant[3] = v3; m_variant[4] = v4; }
    VariantList(Variant v0, Variant v1, Variant v2, Variant v3, Variant v4, Variant v5) { m_variant[0] = v0; m_variant[1] = v1; m_variant[2] = v2; m_variant[3] = v3; m_variant[4] = v4;  m_variant[5] = v5; }
	VariantList(Variant v0, Variant v1, Variant v2, Variant v3, Variant v4, Variant v5, Variant v6) { m_variant[0] = v0; m_variant[1] = v1; m_variant[2] = v2; m_variant[3] = v3; m_variant[4] = v4;  m_variant[5] = v5;  m_variant[6] = v6; }

	// get
	Variant& Get(int parmNum) { return m_variant[parmNum]; }
    Variant* operator[](int parmNum) 
	{
		if (parmNum < 0 || parmNum >= C_MAX_VARIANT_LIST_PARMS)
		{
			return NULL;
		}

        return &m_variant[parmNum];
    }

	// fn
	uint8_t* SerializeToMem(int32_t* pSizeOut);

private:
    Variant m_variant[C_MAX_VARIANT_LIST_PARMS];

};