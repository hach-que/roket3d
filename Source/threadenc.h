#ifndef HEADER_THREADENC
#define HEADER_THREADENC
namespace ext
{
	class ThreadEnc
	{
		public:
			ThreadEnc();
			int start(lua_State* L);
			// So that we can call delete p; on a pointer of type SDLThread
			virtual ~ThreadEnc();
			SDL_Thread *thread;
 
		protected:
			// Pure virtual - _must_ be overridden in a derived class.
			virtual int main() = 0;
			// after this is true, you can no longer change the thread
			bool threadStarted;
 
		private:
			// static method - not related to any single instance
			static int proxy(void *pInst);
 
			ThreadEnc(ThreadEnc const &); // non-copyable
			ThreadEnc &operator=(ThreadEnc const &); // non-copyable
	};
}
#endif
