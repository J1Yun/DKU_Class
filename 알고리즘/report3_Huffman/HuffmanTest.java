
package report3_Huffman;

public class HuffmanTest {
	public static void main(String args[]) throws Exception {
		Huffman huff = new Huffman();
		String str = "";
		TreeNode root = new TreeNode();
		root = huff.HuffmanTree(); // Huffman 트리 생성
		System.out.println("Huffman Code : "); // Huffman 코드 출력
		huff.HuffmanPrint(root, str);
	}
}


