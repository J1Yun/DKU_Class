
package report3_Huffman;

public class TreeNode {
	char symbol; // 알파벳 저장
	int weight; // 빈도수 저장
	TreeNode leftChild;  // 왼쪽 자식
	TreeNode rightChild; // 오른쪽 자식
	TreeNode() {} // 생성자
	TreeNode(TreeNode left, TreeNode right) {
		leftChild = left;
		rightChild = right;
	}
}

