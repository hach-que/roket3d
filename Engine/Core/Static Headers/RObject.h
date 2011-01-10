// Base class for all Roket3D C++ classes.

#ifndef CLASS_RObject
#define CLASS_RObject

class RObject
{
	private:
		long m_ReferenceCount;

	public:
		inline RObject()
		{
			this->m_ReferenceCount = 0;
		}

		inline void Grab()
		{
			this->m_ReferenceCount += 1;
		}

		inline void Drop()
		{
			this->m_ReferenceCount -= 1;

			if (this->m_ReferenceCount <= 0)
				delete this;
		}
};

#endif