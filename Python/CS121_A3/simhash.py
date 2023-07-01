from hashlib import md5


class Sim:
    def __init__(self):
        self.simList = set()

    def simhash(self, freq_map, n=128):
        v = [0] * n  # n-dimensional vector
        # for each token from the document
        for token, weight in freq_map.items():
            # hash the token using md5 cryptographic hash
            h = md5()
            h.update(token.encode("utf-8"))
            # cast hex hash code to int
            code = int(h.hexdigest(), base=16)

            # for each bit in the hash
            for i in range(n):
                # if the ith bit is set
                if code & (1 << i):
                    # add the weight (frequency) of the
                    # token to the ith element of vector v
                    v[i] += weight
                else:
                    # ith bit is not set - subtract
                    # weight (frequency) of the token
                    # from the ith element of vector v
                    v[i] -= weight
        sim = 0
        for i in range(len(v)):  # for each element in v
            if v[i] > 0:  # if ith element of v is positive
                sim |= (1 << i)  # set ith bit of simhash
            else:
                # ith element of v is negative
                sim &= ~(1 << i)  # clear ith bit of simhash
        return sim

    def similarity(self, simhash_a, simhash_b, n=128):
        total = 0
        # for each bit in the simhashes of documents
        # a and b
        for i in range(n):
            # get ith bits of simhashes for a and b
            # bitwise AND (&) ith bit of simhash
            # (possibly returns positive int > 1) - then
            # bitshift right to convert to 1 or 0
            a = (simhash_a & (1 << i)) >> i
            b = (simhash_b & (1 << i)) >> i
            # a XOR b yields 0 if both bits are equal
            # and 1 if not equal - subtract result by 1
            # to negate
            total += (1 - (a ^ b))
        # return ratio of equivalent bits in a and b
        # to total num of bits n
        return total / n

    def compute_word_frequencies(self, tokens):
        freq = {}
        for token in tokens:  # for each token
            # if token is in the map, get count of token
            # and increment - otherwise, add token to
            # the map with a default count of 1
            freq[token] = freq.get(token, 0) + 1
        return freq

    def sim_check(self, tokens):
        freqs = self.compute_word_frequencies(tokens)  # Compute frequencies of word in page
        sim = self.simhash(freqs)  # Use simhash to get hash

        for s in self.simList:  # Check against other page hash and if similarity is higher than 95% return true
            similarity = self.similarity(s, sim)

            if similarity > 0.95:
                return True

        self.simList.add(sim)  # If page is under 95% similar to other pages append to list and return false
        return False