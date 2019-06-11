import timeit
import matplotlib.pyplot as plt

class Paillier:
    
    def __init__(self,fsize):
        self.fsize = fsize//2
    
    def L(self,x):
        n = self.pk[0]
        return (Integer(x)-1)//n
    
    def key_gen(self):
        p = random_prime(2^(self.fsize) - 1, lbound=2^(self.fsize-1))
        q = random_prime(2^(self.fsize) - 1, lbound=2^(self.fsize-1))
        n = p*q
        Zn2 = Zmod(n^2)
        g = Zn2(n + 1)
        Zn = Integers(n)
        l = Zn((p-1)*(q-1))
        inv_l = l.inverse_of_unit()
        self.pk = (n,g)
        self.sk = (l,inv_l)
        return self.pk
    
    @staticmethod
    def encrypt(pk,m):
        n,g = pk
        Zn2 = Integers(n^2)
        ri = randint(1,n-1)
        while gcd(ri,n) != 1:
            ri = randint(1,n-1)
        r = Zn2(ri)    
        return g^m * r^n
    
    def decrypt(self,cipher):
        l,inv_l = self.sk
        n = self.pk[0]
        Zn2 = Integers(n^2)
        Zn = Integers(n)
        return Zn(self.L(Zn2(cipher^l)) * inv_l)
        

class Client:
    def __init__(self, server,ring, M,e_keysize):
        
        
        self.paill = Paillier(e_keysize)
        self.pk = self.paill.key_gen()
        self.r = ring
        self.m = M.nrows()
        self.n = M.ncols()
        self.server = server
        server.send_data(M)
        server.r = r
        v = 0;
        while v==0:                          
            self.u = random_matrix(ring,self.n,1)
            self.t = random_matrix(ring,self.m,1)
            v = M*(self.u) + self.t
        
        w = self.encrypt_vector(v)
        server.w = w
        
    
    def encrypt_vector(self,v):
        w = matrix(Zmod(self.pk[0]^2),v.nrows(), v.ncols()) 
        for k in xrange(0,v.nrows()):
            w[k,0] = Paillier.encrypt(self.pk,v[k,0])
        return w
    
    def audit(self):
        start_autre = timeit.default_timer()
        r = self.r(2*randint(1,self.r.cardinality()) - 1)
        elapsed_autre = timeit.default_timer() - start_autre
        gamma,y = self.server.audit(r)
        start_autre = timeit.default_timer()
        x = vector(self.r, self.m)
        for k in xrange(self.m):
            x[k] = r^(k+1)
            
        elapsed_autre += timeit.default_timer() - start_autre
        #print str(self.r(self.paill.decrypt(gamma))) + "  ,  " + str((y*self.u + x * self.t)[0])
        
        start_decrypt = timeit.default_timer()
        decrypt_delta = self.r(self.paill.decrypt(gamma))
        print "time to decrypt : " + str(timeit.default_timer() - start_decrypt)
        start_autre = timeit.default_timer()
        rep = decrypt_delta == (y*self.u + x * self.t)[0]
        elapsed_autre += timeit.default_timer() - start_autre
        print "time to do other stuff : " + str(elapsed_autre)
        return rep

class Server:
    def __init__(self):
        self.M=0
        self.com_count = 0

    def audit(self,r):
        self.com_count += 1
        start_genx = timeit.default_timer()
        x = vector(self.r, self.M.nrows())
        for k in xrange(self.M.nrows()):
            x[k] = r^(k+1)
            
        elapsed_genx = timeit.default_timer() - start_genx
        print "time to generate x : " + str(elapsed_genx)
        gamma = 1
        start_delta = timeit.default_timer()
        for i in xrange(0, self.w.nrows()):
            gamma *= self.w[i,0]^x[i]
            
        elapsed_delta = timeit.default_timer() - start_delta
        print "time to calculate delta : " + str(elapsed_delta)
        start_y = timeit.default_timer()
        y = x*(self.M)
        elapsed_y = timeit.default_timer() - start_y
        print "time to calculate y : " + str(elapsed_y)
        return gamma,y
    
    def send_data(self,M):
        self.com_count += 1
        self.M = M


#testing time for different field size for 2^30 (1 giga) bits of data

N = 2^30


print "2719307"
r = Zmod(2719307)
n = log(N,2)
m = ceil(N/(floor(log(r.cardinality(),2)) * log(N,2)))
M = random_matrix(r,m,n)
s = Server()
start_init = timeit.default_timer()
c = Client(s,r,M,1024)
elapsed_init = timeit.default_timer() - start_init
print "temps init : " + str(elapsed_init)
start = timeit.default_timer()
c.audit()
elapsed = timeit.default_timer() - start
print "total audit : " + str(elapsed)

print "2^32"
r = Zmod(2^32)
n = log(N,2)
m = ceil(N/(floor(log(r.cardinality(),2)) * log(N,2)))
M = random_matrix(r,m,n)
s = Server()
start_init = timeit.default_timer()
c = Client(s,r,M,1024)
elapsed_init = timeit.default_timer() - start_init
print "temps init : " + str(elapsed_init)
start = timeit.default_timer()
c.audit()
elapsed = timeit.default_timer() - start
print "total audit : " + str(elapsed)

print "2^64"
r = Zmod(2^64)
n = log(N,2)
m = ceil(N/(floor(log(r.cardinality(),2)) * log(N,2)))
M = random_matrix(r,m,n)
s = Server()
start_init = timeit.default_timer()
c = Client(s,r,M,1024)
elapsed_init = timeit.default_timer() - start_init
print "temps init : " + str(elapsed_init)
start = timeit.default_timer()
c.audit()
elapsed = timeit.default_timer() - start
print "total audit : " + str(elapsed)

print "2^128"
r = Zmod(2^128)
n = log(N,2)
m = ceil(N/(floor(log(r.cardinality(),2)) * log(N,2)))
M = random_matrix(r,m,n)
s = Server()
start_init = timeit.default_timer()
c = Client(s,r,M,1024)
elapsed_init = timeit.default_timer() - start_init
print "temps init : " + str(elapsed_init)
start = timeit.default_timer()
c.audit()
elapsed = timeit.default_timer() - start
print "total audit : " + str(elapsed)


print "2^31-1"
r = Zmod(2^31-1)
n = log(N,2)
m = ceil(N/(floor(log(r.cardinality(),2)) * log(N,2)))
M = random_matrix(r,m,n)
s = Server()
start_init = timeit.default_timer()
c = Client(s,r,M,1024)
elapsed_init = timeit.default_timer() - start_init
print "temps init : " + str(elapsed_init)
start = timeit.default_timer()
c.audit()
elapsed = timeit.default_timer() - start
print "total audit : " + str(elapsed)


#looking for the best compromise for the size of the matrix

N = 2^24
times = [[],[],[],[]]
repet = 5
deb = float(10 * 2^20) #10 Mbits/s
pas = log(N,2)
range_k = xrange(floor(log(N,2)),ceil(log(N,2)^3),pas)

print "2719307"
r = Zmod(2719307)
for k in range_k:
    acc = 0
    print "k : " + str(k)
    n = k
    m = ceil(N/(floor(log(r.cardinality(),2))*n))
    M = random_matrix(r,m,n)
    s = Server()
    c = Client(s,r,M,1024)
    for i in xrange(repet):
        start = timeit.default_timer()
        c.audit()
        elapsed = timeit.default_timer() - start
        acc += elapsed
        print "communication times : " + str((n*floor(log(r.cardinality(),2)))/deb)
        print "total audit : " + str(elapsed + (n*floor(log(r.cardinality(),2)))/deb)
    times[0].append(acc/repet + (n*floor(log(r.cardinality(),2)))/deb)

print "2^32"
r = Zmod(2^32)
for k in range_k:
    acc = 0
    print "k : " + str(k)
    n = k
    m = ceil(N/(floor(log(r.cardinality(),2))*n))
    M = random_matrix(r,m,n)
    s = Server()
    c = Client(s,r,M,1024)
    for i in xrange(repet):
        start = timeit.default_timer()
        c.audit()
        elapsed = timeit.default_timer() - start
        acc += elapsed
        print "communication times : " + str((n*floor(log(r.cardinality(),2)))/deb)
        print "total audit : " + str(elapsed + (n*floor(log(r.cardinality(),2)))/deb)
    times[1].append(acc/repet + (n*floor(log(r.cardinality(),2)))/deb)

    
print "2^64"
r = Zmod(2^64)
for k in range_k:
    acc = 0
    print "k : " + str(k)
    n = k
    m = ceil(N/(floor(log(r.cardinality(),2))*n))
    M = random_matrix(r,m,n)
    s = Server()
    c = Client(s,r,M,1024)
    for i in xrange(repet):
        start = timeit.default_timer()
        c.audit()
        elapsed = timeit.default_timer() - start
        acc += elapsed
        print "communication times : " + str((n*floor(log(r.cardinality(),2)))/deb)
        print "total audit : " + str(elapsed + (n*floor(log(r.cardinality(),2)))/deb)
    times[2].append(acc/repet + (n*floor(log(r.cardinality(),2)))/deb)

    
print "2^128"
r = Zmod(2^128)
for k in range_k:
    acc = 0
    print "k : " + str(k)
    n = k
    m = ceil(N/(floor(log(r.cardinality(),2))*n))
    M = random_matrix(r,m,n)
    s = Server()
    c = Client(s,r,M,1024)
    for i in xrange(repet):
        start = timeit.default_timer()
        c.audit()
        elapsed = timeit.default_timer() - start
        acc += elapsed
        print "communication times : " + str((n*floor(log(r.cardinality(),2)))/deb)
        print "total audit : " + str(elapsed + (n*floor(log(r.cardinality(),2)))/deb)
    times[3].append(acc/repet + (n*floor(log(r.cardinality(),2)))/deb)

#print times[0]
range_k2 = xrange(ceil(log(N,2)), ceil(log(N,2)^2), pas)
fig, ax = plt.subplots()
ax.plot(range_k2,times[0][:23],label='2719307')
ax.plot(range_k2,times[1][:23],label='2^32')
ax.plot(range_k2,times[2][:23],label='2^64')
ax.plot(range_k2,times[3][:23],label='2^128')
legend = ax.legend(loc='best')
plt.xlabel("k")
plt.ylabel("time")


