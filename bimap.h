#ifndef BIMAP_H
#define BIMAP_H

#include <cassert>
#include <string>

struct bimap{
	private:
	
	typedef std::string left_t;
	typedef std::string right_t;
	
	struct node {
		left_t l_data;
		node *l_left;
		node *l_right;
		node *l_par;
		
		right_t r_data;
		node *r_left;
		node *r_right;
		node *r_par;
		
		node(left_t const& c, right_t const& v);
	};
	
	node mutable *l_root, *r_root;
	
	template <typename T>
	static node* split(node *&root, T const&comp, T node::*data, node* node::*left, node* node::*right, 
				node* node::*par) {
		node *c, *v, *res = 0;
		c = root;
		root = 0;
		
		while (c) {
			if (c->*data <= comp) {
				v = c->*right;
				c->*right = 0;
				if (!root) {
					c->*par = 0;
					root = c;
				} else {
				    c->*par = root;
					root = root->*right = c;
				}
			} else {
				v = c->*left;
				c->*left = 0;
				if (!res) {
					c->*par = 0;
					res = c;
				} else {
					c->*par = res;
					res = res->*left = c;
				}
			}
			c = v;
		}
		
		if (root) {
			while (root->*par != 0) {
				root = root->*par;
			}
		}
		if (res) {
			while (res->*par != 0) {
				res = res->*par;
			}
		}
		return res;
	}
	
	node* l_split(node *&root, left_t const&comp) const;
	node* r_split(node *&root, right_t const&comp) const;
	
	static node* get_min(node *&root, node* node::*left, node* node::*right, node* node::*par);
	static node* get_max(node *&root, node* node::*left, node* node::*right, node* node::*par);
	
	static node* l_get_min(node *&root);
	static node* l_get_max(node *&root);
	
	static node* r_get_min(node *&root);
	static node* r_get_max(node *&root);
	
	static void merge(node *&first, node *second, node* node::*left, node* node::*right, 
				node* node::*par);
	
	void l_merge(node *&first, node *second) const;
	void r_merge(node *&first, node *second) const;
	
	template <typename T>
	static void next(node* &cur, node *&root, T node::*data, node* node::*left, 
			node* node::*right, node* node::*par) {
		node *c;
		c = split<T>(root, cur->*data, data, left, right, par);
		
		cur = get_min(c, left, right, par);
		
		merge(root, c, left, right, par);
	}
	
	template <typename T>
	static void prev(node* &cur, node* &root, T node::*data, node* node::*left, 
			node* node::*right, node* node::*par) {
		node *c, *v;
		assert(cur != 0);
		c = split<T>(root, cur->*data, data, left, right, par);
		
		if (cur->*par) {
			cur->*par->*right = cur->*left;
			if (cur->*left) {
				cur->*left = cur->*par;
			}
			cur->*par = 0;
			cur->*left = 0;
		} else {
			root = c->*left;
			c->*left = 0;
			if (root) {
				root->*par = 0;
			}
		}
		
		v = cur;
		
		//assert(root);
		
		cur = get_max(root, left, right, par);
		
		merge(root, v, left, right, par);
		merge(root, c, left, right, par);
	}
	
	public:
	
	struct left_iterator;
	struct right_iterator;
	
	struct left_iterator {
		private:
		node* ptr;
		bimap const* root;
		
		left_iterator(node* ptr, bimap const* root);
		
		public:
		
		left_iterator();
		
		bool operator!=(const left_iterator& c);
		bool operator==(const left_iterator& c);
		
		left_t const& operator*() const;
		
		left_iterator& operator++();
		left_iterator operator++(int);
		
		left_iterator& operator--();
		left_iterator operator--(int);
		
		right_iterator flip() const;
		friend bimap;
	};
	struct right_iterator {
		private:
		node* ptr;
		bimap const* root;
		
		right_iterator(node* ptr, bimap const* root);
		public:
			
		right_iterator();
		
		bool operator!=(const right_iterator& c);
		bool operator==(const right_iterator& c);
		
		right_t const& operator*() const;
		
		right_iterator& operator++();
		right_iterator operator++(int);
		
		right_iterator& operator--();
		right_iterator operator--(int);
		
		left_iterator flip() const;
		
		friend bimap;
	};
	
	bimap();
	bimap(const bimap&) = delete;
	void operator=(const bimap&) = delete;
	
	~bimap();
	
	left_iterator insert(left_t const& left, right_t const& right);
	
	void erase(left_iterator it);
	void erase(right_iterator it);
	
	left_iterator  find_left (left_t  const& left)  const;
	right_iterator find_right(right_t const& right) const;
	
	left_iterator begin_left() const;
	left_iterator end_left() const;
	
	right_iterator begin_right() const;
	right_iterator end_right() const;
};

#endif // BIMAP_H
