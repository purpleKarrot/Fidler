
bool
std::string

template<typename T>
std::vector<T>

variant

optional

struct

// -----------------------------------

template<typename T>
void print(std::ostream& out, T);

template<typename T>
void visit_section(std::ostream& out, T);

template<typename T>
void visit_inverted(std::ostream& out, T);


class Context
{
	template<typename T>
	Context(T const& x) :
			model(new Model<T>(x))
	{
	}

private:
	struct Concept
	{
		virtual Concept() = default;
		virtual void do_print(std::ostream& out) const = 0;
		virtual void do_section(std::ostream& out) const = 0;
		virtual void do_inverse(std::ostream& out) const = 0;
	};

	template<typename T>
	struct Model
	{
		Model(T const& x) :
				ref(x)
		{
		}

		virtual void do_print(std::ostream& out) const override
		{
			print(out, ref);
		}

		virtual void do_section(std::ostream& out) const override
		{
			visit_section(out, ref);
		}

		virtual void do_inverse(std::ostream& out) const override
		{
			visit_inverted(out, ref);
		}

		T const& ref;
	};

	friend void print(std::ostream& out, Item const& item)
	{
		item.model->do_print(out);
	}

	friend void visit_section(std::ostream& out, Item const& item)
	{
		item.model->do_section(out);
	}

	friend void visit_inverted(std::ostream& out, Item const& item)
	{
		item.model->do_inverse(out);
	}

	friend Context get_member(std::string const& name);

private:
	std::unique_ptr<Concept const> model;
};


template<typename N, typename Sequence>
Context get_member_impl(Sequence& seq, std::string const& name)
{
	using namespace boost::fusion;
	using namespace boost::fusion::extension;

	Context ret = get_member_impl<N - 1>(seq, name);
	if (!ret && struct_member_name<Sequence, N - 1>::call() == name)
	{
		ret = at_c<N - 1>(seq);
	}
	return ret;
}

template<typename Sequence>
Context get_member_impl<0, Sequence>(Sequence&, std::string const&)
{
	return nullptr;
}

template<typename Sequence>
Context get_member(Sequence& seq, std::string const& name)
{
	return get_member_impl<result_of::size<Sequence>::value>(seq, name);
}





template<typename Sequence>
void visit_member(Sequence const& seq, std::string const& name, )
{

}

struct Visitor
{
	template<typename T>
	void operator()(T& t) const
	{

	}
};



