#ifndef __Format_HeaderPlusPlus__
#define __Format_HeaderPlusPlus__
#include <vector>
#include <fstream>

class Extension;

namespace Format
{
	struct Item
	{
		String name;
		Item(){}
		Item(const Item &from) : name(from.name) {}
		Item(const String &n) : name(n) {}

		virtual Item *Clone() const = 0;
		virtual unsigned Type() const = 0;

		virtual std::basic_ofstream<Char> &Serialize(std::basic_ofstream<Char> &to) const
		{
			return to << name << std::ends << char(Type()), to;
		}
		virtual HANDLE Serialize(HANDLE File) const
		{
			unsigned long written;
			if(!WriteFile(File, name.c_str(), name.size()+1, &written, 0) || written != name.size()+1) return 0;
			char type = char(Type());
			if(!WriteFile(File, &type, sizeof(type), &written, 0) || written != sizeof(type)) return 0;
			return File;
		}
		virtual void Traverse(std::basic_ofstream<Char> &to, Extension &ext) const = 0;
		virtual void Traverse(std::basic_ifstream<Char> &from, Extension &ext) const = 0;

		virtual ~Item(){}
	private:
		Item &operator=(const Item &);
	};
	struct ItemGroup : Item
	{
		typedef std::vector<Item*> items_t;
		items_t items;
		ItemGroup(){}
		ItemGroup(const ItemGroup &from) : Item(from), items(from.CloneItems()) {}
		ItemGroup(const String &n) : Item(n) {}

		virtual ItemGroup *Clone() const
		{
			return new ItemGroup(*this);
		}
		static unsigned const TypeID = 0;
		virtual unsigned Type() const { return TypeID; }

		virtual std::basic_ofstream<Char> &Serialize(std::basic_ofstream<Char> &to) const
		{
			items_t::size_type size = items.size();
			Item::Serialize(to).write((Char*)&size, sizeof(size)/sizeof(Char));
			for(items_t::const_iterator it = items.begin(); it != items.end(); ++it)
			{
				(*it)->Serialize(to);
			}
			return to;
		}
		virtual HANDLE Serialize(HANDLE File) const
		{
			if(!Item::Serialize(File)) return 0;
			unsigned long written;
			items_t::size_type size = items.size();
			if(!WriteFile(File, &size, sizeof(size), &written, 0) || written != sizeof(size)) return 0;
			for(items_t::const_iterator it = items.begin(); it != items.end(); ++it)
			{
				if(!((*it)->Serialize(File))) return 0;
			}
			return File;
		}
		ItemGroup(std::basic_ifstream<Char> &from, const String &nam); //Deserialize
		ItemGroup(HANDLE File, const String &nam); //Deserialize
		virtual void Traverse(std::basic_ofstream<Char> &to, Extension &ext) const;
		virtual void Traverse(std::basic_ifstream<Char> &from, Extension &ext) const;

		virtual ~ItemGroup()
		{
			for(items_t::iterator it = items.begin(); it != items.end(); ++it)
			{
				delete (*it);
			}
		}
	private:
		ItemGroup &operator=(const ItemGroup &);
		items_t CloneItems() const
		{
			items_t temp (items);
			for(items_t::iterator it = temp.begin(); it != temp.end(); ++it)
			{
				*it = (*it)->Clone();
			}
			return temp;
		}
	};
	struct Integer : Item
	{
		unsigned size;
		Integer() : size(4) {}
		Integer(const Integer &from) : Item(from), size(from.size) {}
		Integer(const String &n, unsigned s = 4) : Item(n), size(s) {}

		virtual Integer *Clone() const
		{
			return new Integer(*this);
		}
		static unsigned const TypeID = 1;
		virtual unsigned Type() const { return TypeID; }

		virtual std::basic_ofstream<Char> &Serialize(std::basic_ofstream<Char> &to) const
		{
			return Item::Serialize(to).write((Char*)&size, sizeof(size)/sizeof(Char)), to;
		}
		virtual HANDLE Serialize(HANDLE File) const
		{
			if(!Item::Serialize(File)) return 0;
			unsigned long written;
			if(!WriteFile(File, &size, sizeof(size), &written, 0) || written != sizeof(size)) return 0;
			return File;
		}
		Integer(std::basic_ifstream<Char> &from, const String &n) : Item(n) //Deserialize
		{
			from.read((Char*)&size, sizeof(size)/sizeof(Char));
		}
		Integer(HANDLE File, const String &n) : Item(n) //Deserialize
		{
			unsigned long written;
			if(!ReadFile(File, &size, sizeof(size), &written, 0) || written != sizeof(size)) throw this;
		}
		virtual void Traverse(std::basic_ofstream<Char> &to, Extension &ext) const;
		virtual void Traverse(std::basic_ifstream<Char> &from, Extension &ext) const;

		virtual ~Integer(){}
	private:
		Integer &operator=(const Integer &);
	};
	struct Float : Item
	{
		bool doub;
		Float() : doub(true) {}
		Float(const Float &from) : Item(from), doub(from.doub) {}
		Float(const String &n, bool d = true) : Item(n), doub(d) {}

		virtual Float *Clone() const
		{
			return new Float(*this);
		}
		static unsigned const TypeID = 2;
		virtual unsigned Type() const { return TypeID; }

		virtual std::basic_ofstream<Char> &Serialize(std::basic_ofstream<Char> &to) const
		{
			return Item::Serialize(to) << char(doub), to;
		}
		virtual HANDLE Serialize(HANDLE File) const
		{
			if(!Item::Serialize(File)) return 0;
			unsigned long written;
			char d (doub);
			if(!WriteFile(File, &d, sizeof(d), &written, 0) || written != sizeof(d)) return 0;
			return File;
		}
		Float(std::basic_ifstream<Char> &from, const String &n) : Item(n) //Deserialize
		{
			Char d;
			from.read(&d, sizeof(d)/sizeof(Char));
			doub = d?true:false;
		}
		Float(HANDLE File, const String &n) : Item(n) //Deserialize
		{
			unsigned long written;
			char d;
			if(!ReadFile(File, &d, sizeof(d), &written, 0) || written != sizeof(d)) throw this;
			doub = d?true:false;
		}
		virtual void Traverse(std::basic_ofstream<Char> &to, Extension &ext) const;
		virtual void Traverse(std::basic_ifstream<Char> &from, Extension &ext) const;

		virtual ~Float(){}
	private:
		Float &operator=(const Float &);
	};
	struct Str : Item
	{
		bool unicode;
		Str() : unicode(false) {}
		Str(const Str &from) : Item(from), unicode(from.unicode) {}
		Str(const String &n, bool u = false) : Item(n), unicode(u) {}

		virtual Str *Clone() const
		{
			return new Str(*this);
		}
		static unsigned const TypeID = 3;
		virtual unsigned Type() const { return TypeID; }

		virtual std::basic_ofstream<Char> &Serialize(std::basic_ofstream<Char> &to) const
		{
			return Item::Serialize(to) << char(unicode), to;
		}
		virtual HANDLE Serialize(HANDLE File) const
		{
			if(!Item::Serialize(File)) return 0;
			unsigned long written;
			char u (unicode);
			if(!WriteFile(File, &u, sizeof(u), &written, 0) || written != sizeof(u)) return 0;
			return File;
		}
		Str(std::basic_ifstream<Char> &from, const String &n) : Item(n) //Deserialize
		{
			Char u;
			from.read(&u, sizeof(u)/sizeof(Char));
			unicode = u?true:false;
		}
		Str(HANDLE File, const String &n) : Item(n) //Deserialize
		{
			unsigned long written;
			char u;
			if(!ReadFile(File, &u, sizeof(u), &written, 0) || written != sizeof(u)) throw this;
			unicode = u?true:false;
		}
		virtual void Traverse(std::basic_ofstream<Char> &to, Extension &ext) const;
		virtual void Traverse(std::basic_ifstream<Char> &from, Extension &ext) const;

		virtual ~Str(){}
	private:
		Str &operator=(const Str &);
	};
	struct Raw : Item
	{
		Raw(){}
		Raw(const Raw &from) : Item(from) {}
		Raw(const String &n) : Item(n) {}

		virtual Raw *Clone() const
		{
			return new Raw(*this);
		}
		static unsigned const TypeID = 4;
		virtual unsigned Type() const { return TypeID; }

		using Item::Serialize; //both overloads
		Raw(std::basic_ifstream<Char> &from, const String &n) : Item(n) //Deserialize
		{
		}
		Raw(HANDLE File, const String &n) : Item(n) //Deserialize
		{
		}
		virtual void Traverse(std::basic_ofstream<Char> &to, Extension &ext) const;
		virtual void Traverse(std::basic_ifstream<Char> &from, Extension &ext) const;

		virtual ~Raw(){}
	private:
		Raw &operator=(const Raw &);
	};
}

#endif
