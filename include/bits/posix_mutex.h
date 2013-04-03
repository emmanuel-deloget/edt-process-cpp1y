#ifndef __bits_posix_mutex_h_included
#define __bits_posix_mutex_h_included

#include <utility>
#include <thread>

#include <sys/types.h>

#include <bits/gen_hash.h>
#include <bits/posix_all.h>

namespace std {

	namespace __bits {

		extern void *__shmval_open_and_map(const char *__n, size_t __s, bool& __o);
		extern void __shmval_unmap_and_close(const char *__n, bool __o, void *__p, size_t __s);

		template <class _Type>
		_Type *__shmval_open_and_map(const std::string& __n, bool& __o)
		{
			void *__p = __shmval_open_and_map(__n.c_str(), sizeof(_Type), __o);
			return static_cast<_Type*>(__p);
		}
		
		template <class _Type>
		void __shmval_unmap_and_close(const std::string& __n, bool __o, _Type *__p)
		{
			__shmval_unmap_and_close(__n.c_str(), __o, __p, sizeof(_Type));
		}

		template <class _Type>
		class __shmval
		{
		public:
			typedef _Type value_type;
			typedef _Type& reference;
			typedef const _Type& const_reference;
			typedef value_type* ptr_type;
			typedef value_type const * const ptr_const_type;
			typedef string name_type;

			__shmval(const name_type& __n)
			: _M_name(__n), _M_value(), _M_own()
			{
				_M_value = __shmval_open_and_map<value_type>(_M_name, _M_own);
				if (_M_own)
					*_M_value = value_type();
			}
			~__shmval()
			{
				__shmval_unmap_and_close(_M_name, _M_own, _M_value);
			}

			ptr_type operator->()
			{ return _M_value; }
			
			ptr_const_type operator->() const
			{ return _M_value; }
			
			name_type name() const
			{ return _M_name; }
			
			bool owned() const noexcept
			{ return _M_own; }

		private:
			name_type _M_name;
			ptr_type _M_value;
			bool _M_own;
		};

		template <>
		class __nmimpl<__posix_tag>
		{
		private:
			struct __futex_data
			{
				int _M_val = 1;
				size_t _M_hash;
			};

			typedef __shmval<__futex_data> data_type;

			data_type _M_data;
			size_t _M_h0;

			size_t make_h0() const noexcept
			{
				auto __this = this;
				size_t __h0 = __fnv1_hash_bytes()(&__this, sizeof(__this));
				return __h0;
			}
			
			size_t make_hash() const noexcept
			{
				pid_t __tid = __gettid();
				size_t __h1 = __fnv1_hash_bytes()(&__tid, sizeof(__tid));
				return _M_h0 ^ __h1;
			}
			
		public:
			typedef __nmimpl<__posix_tag> __mutex_type;
			typedef void const * const native_handle_type;

			native_handle_type native_handle() const noexcept
			{ return this; }

			__nmimpl(const std::string& __name)
			: _M_data(__name), _M_h0(make_h0())
			{ }

			~__nmimpl()
			{ }

			size_t hash() const noexcept
			{ return make_hash(); }

			void lock()
			{ 
				while (!try_lock()) {
					__futex(&(_M_data->_M_val), __futex_op::wait, 0, 0);
				}
			}

			void unlock()
			{ 
				if (__sync_fetch_and_or(&(_M_data->_M_val), 0) == 0) {
					// the lock is held - maybe by us?
					if (_M_data->_M_hash == make_hash() && __sync_val_compare_and_swap(&(_M_data->_M_val), 0, 1) == 0) {
						__futex(&(_M_data->_M_val), __futex_op::wake, 100, 0);
						this_thread::yield();
					}
				}
			}

			bool try_lock()
			{
				int __r = __sync_val_compare_and_swap(&(_M_data->_M_val), 1, 0);
				if (__r == 1) {
					// lock is aquired; we set up _M_hash
					_M_data->_M_hash = make_hash();
					return true;
				}
				return false;
			}

			std::string name() const
			{ return _M_data.name(); }
		};

	}

}

#endif // __bits_posix_mutex_h_included
