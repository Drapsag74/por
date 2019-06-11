import hashlib
import timeit

class MerkleTree:
    def __init__(self, root=None):
        self.root = root
        self.fg = None
        self.fd = None
        self.nbleafs = 1
        
    def add_leafs(self, leafs):
        if(len(leafs) == 1):
            self.root = hashlib.sha256(str(leafs[0]))
            self.nbleafs = 1
        elif(len(leafs) == 2):
            self.fg = MerkleTree(hashlib.sha256(str(leafs[0])))
            self.fd = MerkleTree(hashlib.sha256(str(leafs[1])))
            self.root = hashlib.sha256(self.fg.root.digest() + self.fd.root.digest())
            self.nbleafs = 2
        else:
            l = floor(log(len(leafs),2))
            right = len(leafs) % 2^l
            if right == 0:
                mid = ceil(len(leafs)/2)
            else:
                mid = len(leafs) - right
                
            self.fg = MerkleTree()
            self.fd = MerkleTree()
            self.fg.add_leafs(leafs[0:mid])
            self.fd.add_leafs(leafs[mid:])
            self.root = hashlib.sha256(self.fg.root.digest() + self.fd.root.digest())
            self.nbleafs = len(leafs)
            
    def get_leaf(self, i, brothers):
        if self.nbleafs == 1:
            return self.root
        else:
            l = floor(log(self.nbleafs,2))
            right = self.nbleafs % 2^l
            if right == 0:
                mid = self.nbleafs/2
            else:
                mid = self.nbleafs - right
            if i < mid:
                brothers.append((self.fd.root, "r"))
                return self.fg.get_leaf(i%mid,brothers)
            else:
                brothers.append((self.fg.root, "l"))
                return self.fd.get_leaf(i%mid,brothers)
        
    def print_tree(self,space):
        if self.root == None:
            return
        
        if self.fd != None:
            self.fd.print_tree(space+15)
        
        print "\n"+" "* space + str(self.root)
        
        if self.fg != None:
            self.fg.print_tree(space+15)

    def update_bloc(self,val, i):
        if self.nbleafs == 1:
            self.root = hashlib.sha256(str(val))
            return self
        
        l = floor(log(self.nbleafs,2))
        right = self.nbleafs % 2^l
        if right == 0:
            mid = self.nbleafs/2
        else:
            mid = self.nbleafs - right

        if i < mid:
            self.fg = self.fg.update_bloc(val,i%mid)
            self.root = hashlib.sha256(self.fg.root.digest() + self.fd.root.digest())
            return self
        else:
            self.fd = self.fd.update_bloc(val, i%mid)
            self.root = hashlib.sha256(self.fg.root.digest() + self.fd.root.digest())
            return self
        
    def __str__(self):
        if self.root == None:
            return ""
        else:
            return self.fg.__str__() + "--" + self.root.hexdigest() + "--" + self.fd.__str__()

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
        self.root = server.send_data(M,self.r)
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
        r = self.r(randint(1,self.r.cardinality()))
        elapsed_autre = timeit.default_timer() - start_autre
        gamma,y = self.server.audit(r)
        x = vector(self.r, self.m)
        for k in xrange(self.m):
            x[k] = r^(k+1)
        #print str(self.r(self.paill.decrypt(gamma))) + "  ,  " + str((y*self.u + x * self.t)[0])
        
        start_verif = timeit.default_timer()
        rep = self.r(self.paill.decrypt(gamma)) == (y*self.u + x * self.t)[0]
        elapsed_verif =  timeit.default_timer() - start_verif
        print "time to do verification : " + str(elapsed_verif)
        return rep
    
    def update(self,a,i,j):
        bloc,brothers = self.server.get_bloc(i,j)
        for k in xrange(len(brothers)-1, -1, -1):
            if k == len(brothers) -1:
                if brothers[k][1] == "r":
                    h = hashlib.sha256(hashlib.sha256(str(bloc)).digest() + brothers[-1][0].digest())
                else:
                    h = hashlib.sha256(brothers[-1][0].digest() + hashlib.sha256(str(bloc)).digest())
            else:
                if brothers[k][1] == "r":
                    h = hashlib.sha256(h.digest() + brothers[k][0].digest())
                else:
                    h = hashlib.sha256(brothers[k][0].digest() + h.digest())
        if h.digest() != self.root.digest():
            print "ALERTE ROUGE : " + h.hexdigest() + "=/=" +self.root.hexdigest() + "(old digest)"
        modif = self.paill.encrypt(self.pk, (a-bloc)*self.u[j][0])
        
        self.root = self.server.update(a,modif,i,j)
        

class Server:
    def __init__(self):
        self.M=0
        self.com_count = 0
        self.mt = MerkleTree()
        

    def audit(self,r):
        self.com_count += 1
        x = vector(self.r, self.M.nrows())
        for k in xrange(self.M.nrows()):
            x[k] = r^(k+1)
            
        gamma = 1
        
        start_delta = timeit.default_timer()
        for i in xrange(0, self.w.nrows()):
            gamma *= self.w[i,0]^x[i]
            
        elapsed_delta = timeit.default_timer() - start_delta
        print "time to calculate delta : " + str(elapsed_delta)
        y = x*(self.M)
        
        return gamma,y
    
    def send_data(self,M,r):
        self.com_count += 1
        self.r = r
        self.M = M
        leafs = []
        for i in xrange(0,M.nrows()):
            for j in xrange(0,M.ncols()):
                leafs.append(M[i,j])
        
        self.mt.add_leafs(leafs)
        return self.mt.root
        
    def get_bloc(self, i,j):
        numbloc = i*self.M.ncols() + j
        brothers = []
        bloc = M[i,j]
        self.mt.get_leaf(numbloc, brothers).hexdigest()
        return bloc, brothers
    
    def update(self,a,modif,i,j):
        self.M[i,j] = a
        self.w[i] = self.w[i]*modif
        return self.mt.update_bloc(a,i*self.M.ncols() + j).root
        


