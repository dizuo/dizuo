// ----------------------------------------------------------------
//	File Name：		Figure.h
//	Description:	显示多叉树
//	Create Time：	4/10/2009 
//	Author：		RenYaFei
//	Email:			dizuo@126.com
// ----------------------------------------------------------------


// ----------------------------------------------------------------
//	Name：			Figure::buildFigure
//	Description:	计算出层次树的每个节点的位置
//					利用圆优化，有节点重合现象
//					offsetX偏移量还是解决不了问题
//					根因是半径不变
//					使用缩小半斤方法
//	Return Value:	void
// ----------------------------------------------------------------

//ROOT one
//{
//	JOINT two
//	{
//		END apple
//		{
//		}
//		JOINT banana
//		{
//			END 1
//			{
//			}
//			END 2
//			{
//			}			
//			END 3
//			{
//			}			
//			END 4
//			{
//			}
//		}
//		END peach
//		{
//		}
//		END grape
//		{
//		}
//	}
//	JOINT three
//	{
//		JOINT china
//		{
//			END 1
//			{
//			}
//			END 2
//			{
//			}			
//			END 3
//			{
//			}			
//			END 4
//			{
//			}
//		}
//		END england
//		{
//		}
//		END amarica
//		{
//		}
//		END ruassian
//		{
//		}
//	}
//}