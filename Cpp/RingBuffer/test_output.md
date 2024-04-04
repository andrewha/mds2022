$${\color{Green}\text{RingBuffer Class Template test}}$$

$${\color{Green}\text{Test on a buffer of integers}}$$

$${\color{Blue}\text{Stage 1. Definition of class fields and Stage 2. Getter method determines the buffer fullness}}$$

1. Instantiate a buffer of size 0
${\color{Red}\text{Failed to allocate buffer: size cannot be zero.}}$

2. Instantiate a buffer of size 5

Buffer size: 5

Empty? true

Inserted count: 0 element(s)

Can insert more: 5 element(s)

Full? false

$${\color{Blue}\text{Stage 3. The }\tt{push()}\text{ method and Stage 4. The }\tt{front()}\text{ and }\tt{back()}\text{ methods}}$$

Current oldest value: ${\color{Red}\text{No front element: buffer is empty.}}$

Current newest value: ${\color{Red}\text{No back element: buffer is empty.}}$

1. Write (push) 6 element(s) to the buffer

Trying to insert an element with value: 0

Inserted count: 1 element(s)

Can insert more: 4 element(s)

Full? false

Current oldest value: 0

Current newest value: 0

Trying to insert an element with value: 1

Inserted count: 2 element(s)

Can insert more: 3 element(s)

Full? false

Current oldest value: 0

Current newest value: 1

Trying to insert an element with value: 2

Inserted count: 3 element(s)

Can insert more: 2 element(s)

Full? false

Current oldest value: 0

Current newest value: 2

Trying to insert an element with value: 3

Inserted count: 4 element(s)

Can insert more: 1 element(s)

Full? false

Current oldest value: 0

Current newest value: 3

Trying to insert an element with value: 4

Inserted count: 5 element(s)

Can insert more: 0 element(s)

Full? true

Current oldest value: 0

Current newest value: 4

Trying to insert an element with value: 5
${\color{Red}\text{Failed to insert element: buffer is full.}}$

$${\color{Blue}\text{Stage 5. The }\tt{pop()}\text{ method}}$$

1. Read (pop) 6 element(s) from the buffer

Trying to read: 0

Inserted count: 4 element(s)

Can insert more: 1 element(s)

Empty? false

Trying to read: 1

Inserted count: 3 element(s)

Can insert more: 2 element(s)

Empty? false

Trying to read: 2

Inserted count: 2 element(s)

Can insert more: 3 element(s)

Empty? false

Trying to read: 3

Inserted count: 1 element(s)

Can insert more: 4 element(s)

Empty? false

Trying to read: 4

Inserted count: 0 element(s)

Can insert more: 5 element(s)

Empty? true

Trying to read:
${\color{Red}\text{Failed to read element: buffer is empty.}}$

$${\color{Green}\text{Continue test on a buffer of chars}}$$

$${\color{Blue}\text{Stage 6. Copy constructor}}$$

1. Instantiate a buffer of size 3 and push 3 chars to it: 'A', 'B', 'C'

Buffer size: 3

Empty? false

Inserted count: 3 element(s)

Can insert more: 0 element(s)

Full? true

2. Create a copy of the original buffer

Buffer size: 3

Empty? false

Inserted count: 3 element(s)

Can insert more: 0 element(s)

Full? true

Read element(s) from the copied buffer: ABC

3. Exit the scope of the copied buffer -- it will now be destroyed

4. Now that the copied buffered is destroyed, check that the original buffer is still safe

Read element(s) from the original buffer: ABC

$${\color{Green}\text{Continue test on a buffer of booleans}}$$

$${\color{Blue}\text{Stage 7. Copy assignment operation}}$$

1. Instantiate a buffer of size 1 and push 1 `true` to it

2. Instantiate another buffer of size 3 and push 3 `false` to it

3. Assign the second buffer to the first buffer and check if the assignment was successful

Read element(s) from the second buffer before assignment: falsefalsefalse

Read element(s) from the second buffer after assignment: true

$${\color{Green}\text{End of test}}$$
