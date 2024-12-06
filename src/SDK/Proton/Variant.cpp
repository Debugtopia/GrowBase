#include <BaseApp.h> // precompiled
#include <cstring>

#include <SDK/Proton/Variant.h>

Variant::Variant() 
{
    m_type = TYPE_UNUSED;
}

Variant::~Variant() 
{
    //
}

void Variant::Set(float var) 
{
    m_type = TYPE_FLOAT; 
    *((float*)m_var) = var;
}

float& Variant::GetFloat() 
{
    if (m_type == TYPE_UNUSED) 
        Set(float(0));

    return *((float*)m_var);
}

const float& Variant::GetFloat() const 
{ 
    return *((float*)m_var); 
}

void Variant::Set(uint32_t var) 
{
    m_type = TYPE_UINT32; 
    *((uint32_t*)m_var) = var;
}

uint32_t& Variant::GetUInt32() 
{
    if (m_type == TYPE_UNUSED) 
        Set(uint32_t(0));

    return *((uint32_t*)m_var);
}
const uint32_t& Variant::GetUInt32() const 
{ 
    return *((uint32_t*)m_var); 
}

void Variant::Set(int32_t var) 
{
    m_type = TYPE_INT32; 
    *((int32_t*)m_var) = var;
}

int32_t& Variant::GetInt32() 
{
    if (m_type == TYPE_UNUSED) 
        Set(int32_t(0));
        
    return *((int32_t*)m_var);
}

const int32_t& Variant::GetInt32() const 
{ 
    return *((int32_t*)m_var); 
}

void Variant::Set(std::string const &var) 
{
	m_type = TYPE_STRING;
	m_string = var;
}

std::string& Variant::GetString() 
{
    return m_string;
}

const std::string& Variant::GetString() const 
{ 
    return m_string; 
}

void Variant::Set(const CL_Vec2f& var) 
{
    m_type = TYPE_VECTOR2; 
    *((CL_Vec2f*)m_var) = var;
}

CL_Vec2f& Variant::GetVector2() 
{
    if (m_type == TYPE_UNUSED)
        Set(CL_Vec2f(0,0));

    return *((CL_Vec2f*)m_var);
}

const CL_Vec2f& Variant::GetVector2() const 
{
    return *((CL_Vec2f*)m_var);
}

void Variant::Set(const CL_Vec3f& var) 
{
    m_type = TYPE_VECTOR3; 
    *((CL_Vec3f*)m_var) = var;
}

CL_Vec3f& Variant::GetVector3() 
{
    if (m_type == TYPE_UNUSED)
        Set(CL_Vec3f(0,0,0));

    return *((CL_Vec3f*)m_var);
}

const CL_Vec3f& Variant::GetVector3() const 
{
    return  *((CL_Vec3f*)m_var);
}

int GetSizeOfData(Variant::eType type) 
{
	switch(type)
	{
	case Variant::TYPE_UINT32:
	case Variant::TYPE_INT32:
	case Variant::TYPE_FLOAT:
		return 4;
	case Variant::TYPE_VECTOR2:
		return sizeof(CL_Vec2f);
	case Variant::TYPE_VECTOR3:
		return sizeof(CL_Vec3f);
	default:
        return 0;
	}
}

uint8_t* VariantList::SerializeToMem(int32_t* pSizeOut)
{
	int varsUsed = 0;
	int memNeeded = 0;
	int tempSize;

	for (int i = 0; i < C_MAX_VARIANT_LIST_PARMS; i++)
	{
		if (m_variant[i].GetType() == Variant::TYPE_STRING) 
        {
            tempSize = (int)m_variant[i].GetString().size() + 4;
		} 
        else 
        {
			tempSize = GetSizeOfData(m_variant[i].GetType());
		}

		if (tempSize <= 0)
        {
            continue;
        }

        varsUsed++;
        memNeeded += tempSize;
	}

	int totalSize = memNeeded + 1 + (varsUsed * 2);
	uint8_t* pBuffer = (uint8_t*)malloc(totalSize);
	uint8_t* pCur = pBuffer;

	pCur[0] = uint8_t(varsUsed);
	pCur++;

	uint8_t type;
	for (int i = 0; i < C_MAX_VARIANT_LIST_PARMS; i++)
	{
		if (m_variant[i].GetType() == Variant::TYPE_STRING) 
        {
			type = i;
			memcpy(pCur, &type, 1); pCur += 1;
			type = Variant::TYPE_STRING;
			memcpy(pCur, &type, 1); pCur += 1;

			uint32_t slength = (int)m_variant[i].GetString().size();
			memcpy(pCur, &slength, 4); pCur += 4;
			memcpy(pCur, m_variant[i].GetString().c_str(), slength); pCur += slength;
		} 
        else 
        {
			tempSize = GetSizeOfData(m_variant[i].GetType());
            if (tempSize <= 0)
            {
                continue;
            }
                
            type = i;
            memcpy(pCur, &type, 1); pCur += 1;
            type = m_variant[i].GetType();
            memcpy(pCur, &type, 1); pCur += 1;
            memcpy(pCur, m_variant[i].m_var, tempSize); pCur += tempSize;
		}
	}

	*pSizeOut = totalSize;
	return pBuffer;
}