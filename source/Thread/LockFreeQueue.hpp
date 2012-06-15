#ifndef LOCKFREEQUEUE_HPP
#define LOCKFREEQUEUE_HPP

#include <atomic>
#include <memory>

namespace ProjectMaya {

	/**
	 * A lock free, high performance queue
	 *
	 * See following paper for more details:
	 * http://people.csail.mit.edu/edya/publications/OptimisticFIFOQueue-journal.pdf
	 */
	template <typename T>
	class LockFreeQueue {
		public:
			LockFreeQueue() {
				Node* nd = new Node;
				nd->next = {nullptr, 0};

				this->tail.store(new Pointer({nd, 0}));
				this->head.store(new Pointer({nd, 0}));
			}

			~LockFreeQueue() {
				// dequeue all elements
				std::shared_ptr<T> val;
				do {
					val = this->dequeue();
				} while (val.get() != nullptr);

				// delete helper
				delete this->tail.load()->ptr;
				delete this->tail.load();
				delete this->head.load();
			}

			/// Adds new element to the queue
			void enqueue(std::shared_ptr<T> val) {
				Pointer* tail;
				Node* nd = new Node;
				nd->value = val;

				while (true) {
					tail = this->tail.load();
					nd->next = {tail->ptr, tail->tag + 1};

					// CAS
					Pointer* expected = tail;
					Pointer* new_val = new Pointer({nd, tail->tag + 1});
					if (this->tail.compare_exchange_weak(expected, new_val)) {
						tail->ptr->prev = {nd, tail->tag};
						delete tail;
						break;
					} else {
						delete new_val;
					}
				}
			}

			/// Get element from queue, returns shared_ptr to nullptr if queue is empty
			std::shared_ptr<T> dequeue() {
				Pointer* tail;
				Pointer* head;
				Pointer firstNodePrev;
				std::shared_ptr<T> val;

				while (true) {
					head = this->head.load();
					tail = this->tail.load();
					firstNodePrev = head->ptr->prev;
					if (head == this->head.load()) {
						if (*tail != *head) {
							if (firstNodePrev.tag != head->tag) {
								this->fixList(tail, head);
								continue;
							}
							val = firstNodePrev.ptr->value;

							// CAS
							Pointer* expected = head;
							Pointer* new_value = new Pointer({firstNodePrev.ptr, head->tag + 1});
							if (this->head.compare_exchange_weak(expected, new_value)) {
								delete head->ptr;
								delete head;
								return val;
							} else {
								delete new_value;
							}
						} else {
							return nullptr;
						}
					}
				}
			}

		private:
			struct Node;
			struct Pointer {
				Node* ptr;
				unsigned int tag;

				bool operator==(const Pointer& obj) {
					return ((this->ptr == obj.ptr) && (this->tag == obj.tag));
				}

				bool operator!=(const Pointer& obj) {
					return ((this->ptr != obj.ptr) || (this->tag != obj.tag));
				}
			};

			struct Node {
				std::shared_ptr<T> value;
				Pointer next;
				Pointer prev;
			};

			std::atomic<Pointer*> head;
			std::atomic<Pointer*> tail;

			void fixList(Pointer* tail, Pointer* head) {
				Pointer curNode;
				Pointer curNodeNext;
				curNode = *tail;
				while((*head == *this->head.load()) && (curNode != *head)) {
					curNodeNext = curNode.ptr->next;
					curNodeNext.ptr->prev = {curNode.ptr, curNode.tag + 1};
					curNode = {curNodeNext.ptr, curNode.tag - 1};
				}
			}
	};

}

#endif

