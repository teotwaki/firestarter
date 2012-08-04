/**
 * .file mirror/utils/wx_gui_factory/data.hpp
 * .brief Class containing data for the wxWidgets-based factory template
 *
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_WX_GUI_FACTORY_DATA_1011291729_HPP
#define MIRROR_UTILS_WX_GUI_FACTORY_DATA_1011291729_HPP

#include <mirror/config.hpp>
#include <mirror/utils/wx_gui_factory/state_saver.hpp>

#include <wx/wx.h>
#include <wx/sizer.h>
#include <wx/xml/xml.h>

#include <memory>
#include <vector>

MIRROR_NAMESPACE_BEGIN

class wx_input_gui_data
{
protected:
	class pub_intf
	{
	public:
		virtual ~pub_intf(void)
		{ }

		// Returns the window associated with this data object
		virtual wxWindow* window(void) const = 0;

		// Returns the sizer associated with this data object
		virtual wxSizer* sizer(void) const = 0;

		// Returns a locally unique identifier string
		// The string is guaranteed to be unique among the siblings
		virtual wxString luid(void) const = 0;

		// Returns the count of children objects of this data object
		virtual std::size_t child_count(void) const = 0;

		// Clears the user input in this and all the child objects
		virtual void clear_input(void) = 0;

		// Initializes the navigation (TAB) chain
		// This member should be called on the topmost data node
		// in the hierarchy
		virtual void init_navigation(void) = 0;

		// Focus the previous input in the navigation chain
		virtual void focus_prev(void) = 0;

		// Focus the next input in the navigation chain
		virtual void focus_next(void) = 0;

		// Save the state of the input widgets to the xml node
		virtual void save_state(wxXmlNode* xml_node) = 0;

		// Save the state of the input widgets to an XML document
		void save_state(wxXmlDocument& xml_doc)
		{
			// TODO: XML namespace
			wxXmlNode* root = new wxXmlNode(
				(wxXmlNode*)nullptr,
				wxXML_ELEMENT_NODE,
				wxT("dialog_state")
			);
			xml_doc.SetRoot(root);
			save_state(root);
		}
	};

	// The basic interface and partial implementation of
	// the data object for a single input unit
	class base_data : public pub_intf
	{
	protected:
		// this class in non-copyable bacause that would
		// wreck the registering process
		base_data(const base_data&);
		//
		// the parent of this data unit
		base_data* parent_data;
		// the zero-based index of this data unit
		std::size_t index;
		// the id
		wxString id;

		virtual void register_child(
			base_data* child,
			std::size_t child_index
		) = 0;

		virtual void unregister_child(
			const base_data* child,
			std::size_t child_index
		) = 0;
	public:
		base_data(
			base_data* data,
			std::size_t idx,
			const wxString& data_id
		): parent_data(data)
		 , index(idx)
		 , id(data_id)
		{
			// if the have a parent data object
			if(parent_data)
			{
				// register this as a child
				// with the specified index
				parent_data->register_child(
					this,
					index
				);
			}
		}

		virtual ~base_data(void)
		{
			// if we have a parent data object
			if(parent_data)
			{
				// unregister this object
				parent_data->unregister_child(
					this,
					index
				);
			}
		}

		// Returns a pointer to the parent data (if any)
		inline base_data* parent(void) const
		{
			return parent_data;
		}

		inline wxString luid(void) const
		{
			return id;
		}

		// Returns the i-th child data object of this object
		virtual base_data* child(std::size_t child_index) const = 0;

		// Returns the count of the currently visible children
		virtual std::size_t visible_child_count(void) const
		{
			return child_count();
		}

		// Returns the j-th visible child data object
		virtual base_data* visible_child(std::size_t child_index) const
		{
			return child(child_index);
		}

		// Returns the sizer associated with this data object
		virtual wxSizer* sizer(void) const
		{
			return nullptr;
		}

		virtual void set_next_in_navigation(base_data* next) = 0;

		// initializes the TAB navigation chain
		virtual base_data* init_navigation_chain(base_data* prev) = 0;

		virtual base_data* get_prev_in_navigation(bool) = 0;

		virtual base_data* get_next_in_navigation(bool) = 0;

		void focus_unit(base_data* data)
		{
			assert(data != nullptr);
			data->window()->SetFocusFromKbd();
		}

		void init_navigation(void)
		{
			init_navigation_chain(this)->set_next_in_navigation(this);
			focus_unit(get_next_in_navigation(false));
		}

		void focus_prev(void)
		{
			focus_unit(get_prev_in_navigation(true));
		}

		void focus_next(void)
		{
			focus_unit(get_next_in_navigation(true));
		}

	};

	template <typename WxWidget>
	class leaf : public base_data
	{
	private:
		WxWidget* leaf_window;
		base_data* prev_in_navigation;
		base_data* next_in_navigation;
		wx_input_gui_state_saver<WxWidget> state_saver;
	protected:
		void register_child(
			base_data* child,
			std::size_t child_index
		)
		{
			// Registering children in a leaf is an error
			assert(!"Leaf data cannot have children");
		}

		void unregister_child(
			const base_data* child,
			std::size_t child_index
		)
		{
			// Unregistering children in a leaf is an error
			assert(!"Leaf data cannot have children");
		}

		// Fallback (do-nothing) implementation of do_clear_input
		static inline void do_clear_input(...)
		{ }

		// Cleat the input of a wxTextCtrl
		static inline void do_clear_input(wxTextCtrl* ctl)
		{
			assert(ctl != nullptr);
			ctl->Clear();
		}
	public:
		leaf(
			base_data* data,
			WxWidget* window,
			std::size_t idx,
			const wxString& id
		): base_data(data, idx, id)
		 , leaf_window(window)
		 , prev_in_navigation(nullptr)
		 , next_in_navigation(nullptr)
		{ }

		std::size_t child_count(void) const
		{
			// no children
			return 0;
		}

		base_data* child(std::size_t child_index) const
		{
			assert(!"Leaf data do not have children");
			return nullptr;
		}

		wxWindow* window(void) const
		{
			return leaf_window;
		}

		void set_next_in_navigation(base_data* next)
		{
			assert(next != nullptr);
			this->next_in_navigation = next;
		}

		base_data* init_navigation_chain(base_data* prev)
		{
			assert(prev != nullptr);
			this->prev_in_navigation = prev;
			prev->set_next_in_navigation(this);
			return this;
		}

		base_data* get_prev_in_navigation(bool first_try)
		{
			if(!first_try) return this;
			assert(prev_in_navigation != nullptr);
			if(prev_in_navigation == this) return this;
			return prev_in_navigation->
				get_prev_in_navigation(false);
		}

		base_data* get_next_in_navigation(bool first_try)
		{
			if(!first_try) return this;
			assert(next_in_navigation != nullptr);
			if(next_in_navigation == this) return this;
			return next_in_navigation->
				get_next_in_navigation(false);
		}

		void clear_input(void)
		{
			// use the specialized function to
			// clear the input in the leaf window
			do_clear_input(leaf_window);
		}

		void save_state(wxXmlNode* parent_node)
		{
			assert(parent_node != nullptr);
			wxXmlNode* new_node = new wxXmlNode(
				parent_node,
				wxXML_ELEMENT_NODE,
				luid()
			);
			assert(new_node != nullptr);
			wxString value;
			if(state_saver.save_state(leaf_window, value))
			{
				new wxXmlNode(
					new_node,
					wxXML_TEXT_NODE,
					wxEmptyString,
					value
				);
			}
		}
	};

	template <typename WxWidget>
	class node : public base_data
	{
	protected:
		WxWidget* node_window;
		wxSizer* node_sizer;
		base_data* prev_in_navigation;
		base_data* next_in_navigation;
	protected:
		// list of pointers to children (not owning the objects)
		std::vector<base_data*> children;

		void register_child(
			base_data* child,
			std::size_t child_index
		)
		{
			// if there are not enough elements in the vector
			if(child_index >= children.size())
			{
				// fill the vector with null pointers
				// so we can access the right one
				// directly by index later
				children.resize(
					(child_index + 1),
					(base_data*)nullptr
				);
			}
			// there must be no other child with this index
			assert(children[child_index] == nullptr);
			// register the child
			children[child_index] = child;
		}

		void unregister_child(
			const base_data* child,
			std::size_t child_index
		)
		{
			// the size of the vector must be ok
			assert(child_index < children.size());
			// and the child must have been registered
			// at the specified index
			assert(children[child_index] == child);
			// unregister the child
			children[child_index] = nullptr;
		}

		// Fallback do nothing implementation of do_clear_input
		static void do_clear_input(...)
		{ }

		static void do_save_state(wxXmlNode* node, ...)
		{ }
	public:
		node(
			base_data* data,
			WxWidget* window,
			wxSizer* sizer,
			std::size_t idx,
			const wxString& id
		): base_data(data, idx, id)
		 , node_window(window)
		 , node_sizer(sizer)
		 , prev_in_navigation(nullptr)
		 , next_in_navigation(nullptr)
		{ }

		base_data* child(std::size_t child_index) const
		{
			return children[child_index];
		}

		std::size_t child_count(void) const
		{
			return children.size();
		}

		wxWindow* window(void) const
		{
			return node_window;
		}

		wxSizer* sizer(void) const
		{
			return node_sizer;
		}

		void set_next_in_navigation(base_data* next)
		{
			next_in_navigation = next;
			assert(next_in_navigation != nullptr);
		}

		base_data* init_navigation_chain(base_data* prev)
		{
			prev_in_navigation = parent() ? parent() : prev;
			assert(prev_in_navigation != nullptr);
			prev->set_next_in_navigation(this);
			prev = this;
			std::size_t i = 0, n = children.size();
			while(i != n)
			{
				assert(children[i] != nullptr);
				prev = children[i]->init_navigation_chain(prev);
				++i;
			}
			return prev;
		}

		base_data* get_prev_in_navigation(bool first_try)
		{
			assert(prev_in_navigation != nullptr);
			if(prev_in_navigation == this) return this;
			return prev_in_navigation->
				get_prev_in_navigation(first_try);
		}

		base_data* get_next_in_navigation(bool first_try)
		{
			if(!children.empty())
				return children[0]->
					get_next_in_navigation(false);
			assert(next_in_navigation != nullptr);
			if(next_in_navigation == this) return this;
			return next_in_navigation->
				get_next_in_navigation(first_try);
		}

		void clear_input(void)
		{
			std::size_t i = 0, n = children.size();
			while(i != n)
			{
				assert(children[i] != nullptr);
				children[i]->clear_input();
				++i;
			}
			do_clear_input(node_window);
		}

		void save_state(wxXmlNode* parent_node)
		{
			assert(parent_node != nullptr);
			wxXmlNode* new_node = new wxXmlNode(
				parent_node,
				wxXML_ELEMENT_NODE,
				luid()
			);
			assert(new_node != nullptr);
			std::size_t i = 0, n = children.size();
			while(i != n)
			{
				assert(children[i] != nullptr);
				children[i]->save_state(new_node);
				++i;
			}
			do_save_state(new_node, node_window);
		}
	};

	template <class WxWidget>
	class manager : public node<WxWidget>
	{
	private:
		typedef std::vector<base_data*> data_list;
		data_list tmp_list;
	public:
		manager(
			base_data* data,
			WxWidget* window,
			const wxString& id
		): node<WxWidget>(data, window, (wxSizer*)nullptr, 0, id)
		{ }

		inline bool has_selection(void) const
		{
			return this->node_window->GetSelection() != wxNOT_FOUND;
		}

		inline std::size_t selected(void) const
		{
			assert(has_selection());
			return this->node_window->GetSelection();
		}

		virtual std::size_t visible_child_count(void) const
		{
			return has_selection() ? 1 : 0;
		}

		virtual base_data* visible_child(std::size_t child_index) const
		{
			assert(child_index < visible_child_count());
			assert(selected() < this->child_count());
			return child(selected());
		}

		void set_next_in_navigation(base_data* next)
		{
			assert(next != nullptr);
			if(this->next_in_navigation == nullptr)
			{
				assert(tmp_list.empty());
				this->next_in_navigation = next;
			}
			else
			{
				data_list::const_iterator
					i = this->tmp_list.begin(),
					e = this->tmp_list.end();
				while(i != e)
				{
					(*i)->set_next_in_navigation(next);
					++i;
				}
				this->tmp_list.clear();
			}
		}

		base_data* init_navigation_chain(base_data* prev)
		{
			assert(tmp_list.empty());
			this->prev_in_navigation = prev;
			assert(this->prev_in_navigation != nullptr);
			prev->set_next_in_navigation(this);
			prev = this;
			std::size_t i = 0, n = this->children.size();
			while(i != n)
			{
				assert(this->children[i] != nullptr);
				prev = this->children[i]->
					init_navigation_chain(prev);
				this->tmp_list.push_back(prev);
				++i;
			}
			return this;
		}

		base_data* get_prev_in_navigation(bool first_try)
		{
			if(!first_try) return this;
			assert(this->prev_in_navigation != nullptr);
			return this->prev_in_navigation->
				get_prev_in_navigation(false);
		}

		base_data* get_next_in_navigation(bool first_try)
		{
			if(!first_try) return this;
			if(has_selection())
				return child(selected())->
					get_next_in_navigation(false);
			assert(this->next_in_navigation != nullptr);
			return this->next_in_navigation->
				get_next_in_navigation(false);
		}
	};

	// pointer to the implementation of the data
	std::shared_ptr<base_data> impl;

	wx_input_gui_data(base_data* data)
	 : impl(data)
	{ }
public:
	// weak reference to the data
	class weak_ref : public std::weak_ptr<base_data>
	{
	public:
		weak_ref(void)
		{ }

		weak_ref(const wx_input_gui_data& data)
		 : std::weak_ptr<base_data>(data.impl)
		{ }
	};

	wx_input_gui_data(void)
	{ }

	wx_input_gui_data(const weak_ref& ref)
	 : impl(ref.lock())
	{ }

	// Make a root node data with children
	template <class WxWidget>
	inline wx_input_gui_data(
		WxWidget* window,
		wxSizer* sizer,
		const wxString& id
	): impl(new node<WxWidget>(0, window, sizer, 0, id))
	{ }

	// Make a node data with children
	template <class WxWidget>
	inline wx_input_gui_data(
		const wx_input_gui_data& data,
		WxWidget* window,
		wxSizer* sizer,
		std::size_t idx,
		const wxString& id
	): impl(new node<WxWidget>(data.impl.get(), window, sizer, idx, id))
	{ }

	// Make a manager data object
	template <class WxWidget>
	inline wx_input_gui_data(
		const wx_input_gui_data& data,
		WxWidget* window,
		const wxString& id
	): impl(new manager<WxWidget>(data.impl.get(), window, id))
	{ }

	// Make a leaf data without children
	template <class WxWidget>
	inline wx_input_gui_data(
		const wx_input_gui_data& data,
		WxWidget* window,
		std::size_t idx,
		const wxString& id
	): impl(new leaf<WxWidget>(data.impl.get(), window, idx, id))
	{ }

	inline void clear(void)
	{
		impl.reset();
	}

	inline pub_intf* operator -> (void) const
	{
		assert(impl.get() != nullptr);
		return impl.get();
	}

};

MIRROR_NAMESPACE_END

#endif //include guard

