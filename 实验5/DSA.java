import java.math.BigInteger;
import java.util.Random;


public class DSA {
	
	public static BigInteger p;
	public static BigInteger q;
	public static BigInteger x;
	public static BigInteger y;
	public static BigInteger g;
	public static BigInteger h;
	public static BigInteger k;
	public static BigInteger r;
	public static BigInteger s;
	public static BigInteger w;
	public static BigInteger u1;
	public static BigInteger u2;
    
	public void genParam() {
		p = BigInteger.probablePrime(20, new Random());
		System.out.println("p:"+p.toString());
		
		q = BigInteger.probablePrime(5, new Random());
		System.out.println("q:"+q.toString());
		
		int count = 1;
		while(p.add(BigInteger.ONE.negate()).mod(q)!=BigInteger.ZERO){
			count ++;
			q = q.nextProbablePrime();
			System.out.println("q:"+q.toString());
		}
		System.out.println("共试验q的个数:"+count);
		if(p.add(BigInteger.ONE.negate()).mod(q)!=BigInteger.ZERO)
			System.out.println("不整除");
				
		h = new BigInteger(512, new Random());
		System.out.println("h:"+h.toString());
		
		g = h.modPow(p.add(BigInteger.ONE.negate()).divide(q), p);
		System.out.println("g:"+g.toString());
		
		x = new BigInteger(160, new Random());
		System.out.println("私钥x:"+x.toString());
		
		y = g.modPow(x, p);
		System.out.println("公钥y:"+y.toString());
		
	}
	
	public static void main(String[] args) throws Exception {
		new DSA().genParam();
		k = new BigInteger(160, new Random());
		System.out.println("Alice选取的k:"+k.toString());
		r = g.modPow(k, p).mod(q);
		String str = "abcd";
		System.out.println("要签名的消息:"+str);
		SHA sha = new SHA();
		sha.SignSHA(str);
		BigInteger temp;
		temp = x.multiply(r);
		s = (k.modInverse(q).multiply(sha.digest.add(temp))).mod(q);
		System.out.println("Alice的签名:\n"+"\tr:"+r.toString());
		System.out.println("\ts:"+s.toString());
		w = s.modInverse(q);
		u1 = sha.digest.multiply(w).mod(q);
		u2 = r.multiply(w).mod(q);
		BigInteger v = g.modPow(u1, p).multiply(y.modPow(u2, p)).mod(p).mod(q);
		System.out.println("Bob验证签名:\n"+"\tv:"+v.toString());
		if(v.equals(r)) 
			System.out.println("签名验证成功");
		else
			System.out.println("签名验证失败");
	}
}
