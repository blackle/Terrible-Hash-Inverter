> **basilisk.txt** (also known as **the basilisk file** or **IA PoW 0**<sup>[[1]](https://suricrasia.online)</sup>) is the name of a collection of over 125 million [partial hash inversions](https://en.wikipedia.org/wiki/Partial_hash_inversion) of the [SHA-256](https://en.wikipedia.org/wiki/SHA-2) [cryptographic hash function](https://en.wikipedia.org/wiki/Cryptographic_hash_function). Assuming state-of-the art methods were used to compute the inversions, the entries in the collection collectively represent a [proof-of-work](https://en.wikipedia.org/wiki/Proof_of_work) far exceeding the computational capacity of the human race.<sup>[[2]](https://suricrasia.online)</sup><sup>[[3]](https://suricrasia.online)</sup> The collection first began to appear on the internet sometime before June 2017,<sup>[[3]](https://suricrasia.online)</sup><sup>[[1]](https://suricrasia.online)</sup> however it was not widely reported or discussed until early 2019.<sup>[[3]](https://suricrasia.online)</sup><sup>[[4]](https://suricrasia.online)</sup><sup>[[1]](https://suricrasia.online)</sup>

This hash inverting program was written to generate believable partial hash inversions for a science fiction story.

#### Building:

```
$ make
```

#### Running:

There are two programs generated. `benchmark` will run X number of hashes. This is used to benchmark the hashing speed:

```bash
$ ./benchmark 10000000
80000000 hashes computed
7.97 seconds taken
10.04 million hashes per second
```

The second is `gen`, which will find a nonce value so the string `basilisk:000000000N:<nonce>` gives a hash with eight zeros.

```bash
$ ./gen 0
basilisk:0000000000:qPAZ7WhrpBKrq28XtMxHffnsCYFC74EKlmktoXhBZj0GKnQCQtMYfXxpNib9ak7q 00000000be9f66ca22673497cab50359f21809f91714d26a716252eafcb6964e
```

These are double-applied hashes like those in bitcoin protocol. To verify:

```bash
$ echo -en 'basilisk:0000000000:qPAZ7WhrpBKrq28XtMxHffnsCYFC74EKlmktoXhBZj0GKnQCQtMYfXxpNib9ak7q' | openssl dgst -sha256 -binary | openssl dgst -sha256
(stdin)= 00000000be9f66ca22673497cab50359f21809f91714d26a716252eafcb6964e
```
