// *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*- Importing Libraries  *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

#include "stdio.h"
#include "stdlib.h"
#include <string.h>

#define NAME_SIZE 20
#define ROLL_SIZE 15
#define SUBJECT_SIZE 5

typedef enum
{
    PENDING,
    CLEAR
} FEE_STATUS;
typedef enum
{
    NOTAPPLIED,
    APPLIED
} APPLICANT;
typedef enum
{
    FIRST,
    SECOND,
    THIRD,
    FOURTH
} YEAR;

//  *-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*   Structure Definitions    *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

typedef struct Details_tag
{
    char name[NAME_SIZE];
    char dep[5];
    YEAR year;
    char rollno[ROLL_SIZE];
    int classesConducted[5];
    int classesAttended[5];
    FEE_STATUS fee;
    APPLICANT app;
} Details;
typedef Details Item_Type;

typedef struct Attendance_List_Tag
{
    char rollno[ROLL_SIZE];
    char dep[5];
    float attendance_Percent;
    struct Attendance_List_Tag *left, *right;
} Attendance_List_Node;

typedef struct FeeStatus_List_Tag
{
    char roll[ROLL_SIZE];
    FEE_STATUS fee;
    struct FeeStatus_List_Tag *left, *right;
} FeeStatus_List_Node;
typedef struct ApplicantStatus_List_Tag
{
    char name[NAME_SIZE];
    char roll[ROLL_SIZE];
    APPLICANT apply;
    struct ApplicantStatus_List_Tag *left, *right;
} ApplicantStatus_List_Node;

typedef struct node_tag
{
    Item_Type data;
    struct node_tag *left;
    struct node_tag *right;
    int height;
    //

    Attendance_List_Node *ptr_To_Attendance_List_Node;
    FeeStatus_List_Node *ptr_To_Fee_Status_List_Node;
    ApplicantStatus_List_Node *ptr_To_Applicant_Status_List_Node;

} Tree_Node;

// Maximum classes conducted
int MCC[4][5] =
    {
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1}};
// Maximum classes attended
int MCA[4][5] =
    {
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1}};

// *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*- Utility Functions  *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

void filling_MCC(Item_Type data)
{
    int row = data.year;
    int col = -1;
    char c = data.dep[0];
    switch (c)
    {
    case 'C': // CS or CV
        if (data.dep[1] == 'S')
            col = 0;
        else
            col = 1;
        break;
    case 'E':
        if (data.dep[1] == 'C')
            col = 2;
        else
            col = 3;
        break;
    case 'M':
        col = 4;
        break;
    default:
        printf("\nERROR !!! DEPARTMENT NOT FOUND\n");
        break;
    }
    if (col != -1 && MCC[row][col] == -1) // MEMOIZATION
    {
        int maxclasses_conducted = data.classesConducted[0];

        for (int i = 1; i < 5; i++) // no of subjects
        {
            if (maxclasses_conducted < data.classesConducted[i])
                maxclasses_conducted = data.classesConducted[i];
        }
        MCC[row][col] = maxclasses_conducted;
    }
    // filling MCA for MCC[row][col]
    for (int i = 1; i < 5; i++) // no of subjects
    {
        if (MCC[row][col] == data.classesConducted[i])
        {
            if (MCA[row][col] < data.classesAttended[i])
                MCA[row][col] = data.classesAttended[i];
        }
    }
}

/*To Compare 2 nodes on the basis of year=>department=>roll no
    -1 for 1st node greater, 1 for 2nd node greater, 0 for equal*/
int comparator(Details d1, Details d2)
{
    int ret_val = 0;
    // SORT FROM YEAR 4 TO YEAR 1
    if (d1.year < d2.year)
        ret_val = -1;
    else if (d1.year > d2.year)
        ret_val = 1;
    else // YEAR IS SAME
    {
        if (strcmp(d1.dep, d2.dep) < 0)
            ret_val = 1;
        else if (strcmp(d1.dep, d2.dep) > 0)
            ret_val = -1;
        else // Department is same
        {
            if (strcmp(d1.rollno, d2.rollno) < 0)
                ret_val = 1;
            else if (strcmp(d1.rollno, d2.rollno) > 0)
                ret_val = -1;
        }
    }
    return ret_val;
}

// A utility function to get maximum of two integers
int max(int a, int b)
{
    return (a > b) ? a : b;
}

// A function to print fee status of all Students
void printFeeStatus(Tree_Node *root)
{
    if (root)
    {
        printFeeStatus(root->left);
        printf("\nFee Status is %d for %s ", root->ptr_To_Fee_Status_List_Node->fee, root->ptr_To_Fee_Status_List_Node->roll);
        printFeeStatus(root->right);
    }
}

// A function to print Attendance % of all Students
void printAttendance(Tree_Node *root)
{
    if (root)
    {
        printAttendance(root->left);
        printf("\n%20s\t%s\t%f", root->data.name, root->ptr_To_Attendance_List_Node->rollno, root->ptr_To_Attendance_List_Node->attendance_Percent);
        printAttendance(root->right);
    }
}

// A function to connect all the nodes(to their corresponding nodes in other Lists) of the tree. For eg.GSRL to FSL,ASLand ASRL
void connector(Tree_Node *root)
{
    if (root)
    {
        connector(root->right);
        connector(root->left);
        if (root->left == NULL && root->right == NULL)
        {
            root->ptr_To_Fee_Status_List_Node->left = root->ptr_To_Fee_Status_List_Node->right = NULL;
            root->ptr_To_Applicant_Status_List_Node->left = root->ptr_To_Applicant_Status_List_Node->right = NULL;
            root->ptr_To_Attendance_List_Node->left = root->ptr_To_Attendance_List_Node->right = NULL;
        }
        else if (root->left != NULL && root->right == NULL)
        {
            root->ptr_To_Fee_Status_List_Node->left = root->left->ptr_To_Fee_Status_List_Node;
            root->ptr_To_Applicant_Status_List_Node->left = root->left->ptr_To_Applicant_Status_List_Node;
            root->ptr_To_Attendance_List_Node->left = root->left->ptr_To_Attendance_List_Node;
            //
            root->ptr_To_Fee_Status_List_Node->right = NULL;
            root->ptr_To_Applicant_Status_List_Node->right = NULL;
            root->ptr_To_Attendance_List_Node->right = NULL;
        }
        else if (root->left == NULL && root->right != NULL)
        {
            root->ptr_To_Fee_Status_List_Node->left = NULL;
            root->ptr_To_Applicant_Status_List_Node->left = NULL;
            root->ptr_To_Attendance_List_Node->left = NULL;
            //
            root->ptr_To_Fee_Status_List_Node->right = root->right->ptr_To_Fee_Status_List_Node;
            root->ptr_To_Applicant_Status_List_Node->right = root->right->ptr_To_Applicant_Status_List_Node;
            root->ptr_To_Attendance_List_Node->right = root->right->ptr_To_Attendance_List_Node;
        }
        else
        {
            root->ptr_To_Fee_Status_List_Node->left = root->left->ptr_To_Fee_Status_List_Node;
            root->ptr_To_Applicant_Status_List_Node->left = root->left->ptr_To_Applicant_Status_List_Node;
            root->ptr_To_Attendance_List_Node->left = root->left->ptr_To_Attendance_List_Node;
            //
            root->ptr_To_Fee_Status_List_Node->right = root->right->ptr_To_Fee_Status_List_Node;
            root->ptr_To_Applicant_Status_List_Node->right = root->right->ptr_To_Applicant_Status_List_Node;
            root->ptr_To_Attendance_List_Node->right = root->right->ptr_To_Attendance_List_Node;
        }
    }
}

// A function to fill attendance % of all Students
void fillAttendance(Tree_Node *root)
{
    if (root)
    {
        float att_percent = -1;

        int row = root->data.year;
        int col = -1;
        char c = root->data.dep[0];
        switch (c)
        {
        case 'C': // CS or CV
            if (root->data.dep[1] == 'S')
                col = 0;
            else
                col = 1;
            break;
        case 'E':
            if (root->data.dep[1] == 'C')
                col = 2;
            else
                col = 3;
            break;
        case 'M':
            col = 4;
            break;
        default:
            printf("\nWrong character is %c", c);
            printf("\nERROR !!! DEPARTMENT NOT FOUND\n");
            break;
        }
        for (int i = 0; i < SUBJECT_SIZE; i++)
        {
            if (MCC[row][col] == root->data.classesConducted[i])
            {
                float att_For_Current_Subject = (root->data.classesAttended[i] * 1.0) / (MCA[row][col]) * 100.0;

                if (att_percent < att_For_Current_Subject && att_percent <= 100)
                    att_percent = att_For_Current_Subject;
            }
        }
        root->ptr_To_Attendance_List_Node->attendance_Percent = att_percent;
        fillAttendance(root->left);
        fillAttendance(root->right);
    }
}

// *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-  AVL Tree Functions    *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*--*-*-*-

// To create a new node in AVL tree
Tree_Node *NewNode(Details data)
{
    Tree_Node *temp = (Tree_Node *)malloc(sizeof(Tree_Node));
    temp->data = data;
    temp->left = NULL;
    temp->right = NULL;
    temp->height = 1;
    temp->ptr_To_Applicant_Status_List_Node = (ApplicantStatus_List_Node *)malloc(sizeof(ApplicantStatus_List_Node));
    temp->ptr_To_Attendance_List_Node = (Attendance_List_Node *)malloc(sizeof(Attendance_List_Node));
    temp->ptr_To_Fee_Status_List_Node = (FeeStatus_List_Node *)malloc(sizeof(FeeStatus_List_Node));

    temp->ptr_To_Applicant_Status_List_Node->apply = data.app;
    strcpy(temp->ptr_To_Applicant_Status_List_Node->name, data.name);
    strcpy(temp->ptr_To_Applicant_Status_List_Node->roll, data.rollno);

    temp->ptr_To_Fee_Status_List_Node->fee = data.fee;
    strcpy(temp->ptr_To_Fee_Status_List_Node->roll, data.rollno);

    strcpy(temp->ptr_To_Attendance_List_Node->dep, data.dep);
    strcpy(temp->ptr_To_Attendance_List_Node->rollno, data.rollno);

    return temp;
}

// A function to get the height of the tree
int height(Tree_Node *node)
{
    if (node == NULL)
        return 0;
    return node->height;
}

// A function to get balance factor of node N
int Balance(Tree_Node *node)
{
    if (node == NULL)
        return 0;
    return height(node->left) - height(node->right);
}

// A function to left rotate subtree rooted with z
Tree_Node *LeftRotate(Tree_Node *z)
{
    Tree_Node *y = z->right;
    Tree_Node *t2 = y->left;

    y->left = z;
    z->right = t2;

    z->height = max(height(z->left), height(z->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

// A function to right rotate subtree rooted with z
Tree_Node *RightRotate(Tree_Node *z)
{
    Tree_Node *y = z->left;
    Tree_Node *t3 = y->right;

    y->right = z;
    z->left = t3;

    z->height = max(height(z->left), height(z->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

// A function to print preorder traversal of the tree.
void preorder(Tree_Node *root)
{
    if (root == NULL)
        return;
    printf("\n%20s %s", root->data.name, root->data.rollno);
    preorder(root->left);
    preorder(root->right);
}

// A function to return the node with minimum value in the tree.
Tree_Node *FindMin(Tree_Node *node)
{
    while (node && node->left != NULL)
        node = node->left;
    return node;
}

// A function to delete a node with given data from AVL tree.
Tree_Node *Delete(Tree_Node *root, Item_Type data)
{
    // Base case: If the tree is empty, return NULL
    if (root == NULL)
        return root;
    // Recursive deletion based on comparison with root data
    if (comparator(data, root->data) == 1) // data < root->data
        root->left = Delete(root->left, data);
    else if (comparator(data, root->data) == -1) // data > root->data
        root->right = Delete(root->right, data);
    else
    {
        // Case 1: Node to be deleted has no children (leaf node)
        if (root->right == NULL && root->left == NULL)
        {
            free(root);
            root = NULL;
        }
        // Case 2: Node to be deleted has only left child
        else if (root->left != NULL && root->right == NULL)
        {
            Tree_Node *temp = root;
            root = root->left;
            free(temp);
        }
        // Case 3: Node to be deleted has only right child
        else if (root->right != NULL && root->left == NULL)
        {
            Tree_Node *temp = root;
            root = root->right;
            free(temp);
        }
        // Case 4: Node to be deleted has both left and right children
        else
        {
            Tree_Node *temp = FindMin(root->right);        // Find minimum node in right subtree
            root->data = temp->data;                       // Replace root's data with minimum node's data
            root->right = Delete(root->right, temp->data); // Delete the minimum node from right subtree
        }
    }

    // If root is NULL after deletion, return NULL
    if (root == NULL)
        return root;

    // Update height of current node
    root->height = 1 + max(height(root->left), height(root->right));

    // Rebalance the AVL tree after deletion

    int balance = Balance(root);

    // Left Left Case: Perform a right rotation
    if (balance > 1 && Balance(root->left) >= 0)
        return RightRotate(root);
    // Right Right Case: Perform a left rotation
    else if (balance < -1 && Balance(root->right) <= 0)
        return LeftRotate(root);
    // Left Right Case: Perform left then right rotation
    else if (balance > 1 && Balance(root->left) < 0)
    {
        root->left = LeftRotate(root->left);
        return RightRotate(root);
    }
    // Right Left Case: Perform right then left rotation
    else if (balance < -1 && Balance(root->right) > 0)
    {
        root->right = RightRotate(root->right);
        return LeftRotate(root);
    }
    // If no rotation is needed, return the unchanged node
    else
        return root;
}

// A function to insert a new node with given data in AVL tree.
Tree_Node *Insert(Tree_Node *root, Item_Type data)
{
    // Base case: If the tree is empty, return a new node
    if (root == NULL)
        return NewNode(data);
    // Recursive insertion based on comparison with root data
    if (comparator(data, root->data) == 1) // data < root->data
        root->left = Insert(root->left, data);
    else if (comparator(data, root->data) == -1) // data > root->data
        root->right = Insert(root->right, data);
    else // data already exists in the tree
        return root;

    // Update the height of the current node
    root->height = max(height(root->left), height(root->right)) + 1;

    // Check and balance the AVL tree after insertion

    int balance = Balance(root);

    // Left Left Case: Perform a right rotation
    if (balance > 1 && comparator(data, root->left->data) == 1) // data < root->left->data
        return RightRotate(root);
    // Right Right Case: Perform a left rotation
    else if (balance < -1 && comparator(data, root->right->data) == -1) // data > root->right->data
        return LeftRotate(root);
    // Left Right Case: Perform left then right rotation
    else if (balance > 1 && comparator(data, root->left->data) == -1) // data > root->left->data
    {
        root->left = LeftRotate(root->left);
        return RightRotate(root);
    }
    // Right Left Case: Perform right then left rotation
    else if (balance < -1 && comparator(data, root->right->data) == 1) // data < root->right->data
    {
        root->right = RightRotate(root->right);
        return LeftRotate(root);
    }
    // If no rotation is needed, return the unchanged node
    else
        return root;
}

// inorder traversal of the AVL tree
void inorder(Tree_Node *root1)
{
    if (root1)
    {
        inorder(root1->left);
        printf("\n%20s\t%s ", root1->data.name, root1->data.rollno);
        inorder(root1->right);
    }
}

// To delete the AVL tree
void DeleteAVLTree(Tree_Node *root)
{
    if (root)
    {
        DeleteAVLTree(root->left);
        DeleteAVLTree(root->right);
        root = Delete(root, root->data);
    }
}

//  *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-  Reading Data Via File  -*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

void readStudent(Item_Type *s, FILE *fp)
{
    fscanf(fp, "%s %s %d %s", s->name, s->dep, &s->year, s->rollno);
    for (int i = 0; i < 5; i++)
        fscanf(fp, "%d", &s->classesConducted[i]);
    for (int i = 0; i < 5; i++)
        fscanf(fp, "%d", &s->classesAttended[i]);
    fscanf(fp, "%d %d", &s->fee, &s->app);
}
Tree_Node *inputFromFile(char *filename, Details student, Tree_Node *head)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    while (!feof(fp))
    {
        readStudent(&student, fp);
        student.year = student.year - 1; // TO USE ENUM LOGIC

        filling_MCC(student);
        head = Insert(head, student);
    }

    fclose(fp);
    return head;
}
Tree_Node *takeInput()
{
    Tree_Node *head = NULL;
    Details d;
    head = inputFromFile("sample.txt", d, head);
    return head;
}

//  *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-* Functions Declarations for Student Record List  *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

void printGSRL(Tree_Node *root);
void printAllNonApplicantStudents(Tree_Node *root);
void printAllEligibleStudents(Tree_Node *root);
void printAllLowAttendanceStudents(Tree_Node *root);
Tree_Node *deleteStudentRecord(Tree_Node *root);
void Question6(Tree_Node *root);
void printAllDefaulterStudents(Tree_Node *root);
void printAllStudentsWithinRange(Tree_Node *root);

// *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*- Main *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

int main()
{
    Tree_Node *root_Of_Records = NULL;
    root_Of_Records = takeInput();
    connector(root_Of_Records);

    printf("\nPrinting MCC\n");
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 5; j++)
            printf("\t %d", MCC[i][j]);
        printf("\n");
    }
    printf("\nPrinting MCA\n");
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 5; j++)
            printf("\t %d", MCA[i][j]);
        printf("\n");
    }
    fillAttendance(root_Of_Records);

    int choice = 0;
    while (choice != 9)
    {
        printf("\n\n\n\n\n1:General Student Record Database(sorted).");
        printf("\n2:List of Students who didn't apply for the Examination.");
        printf("\n3:List of all Eligible Students for Examination.");
        printf("\n4:List of Students having Attendance <=75%%.");
        printf("\n5:Delete a Student Record from Database.");
        printf("\n6:List of Students whose attendance is >75%% for the respective MCC(Maximum Classes Conducted) but their Fee Status is PENDING.");
        printf("\n7:List of Defaulter Students.");
        printf("\n8:Range-search students based on the roll number.");
        printf("\n9:Exit\n");
        printf("\nEnter choice\n");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            printGSRL(root_Of_Records);
            break;
        case 2:
            printAllNonApplicantStudents(root_Of_Records);
            break;
        case 3:
            printAllEligibleStudents(root_Of_Records);
            break;
        case 4:
            printAllLowAttendanceStudents(root_Of_Records);
            break;
        case 5:
            root_Of_Records = deleteStudentRecord(root_Of_Records);
            break;
        case 6:
            Question6(root_Of_Records);
            break;
        case 7:
            printAllDefaulterStudents(root_Of_Records);
            break;
        case 8:
            printAllStudentsWithinRange(root_Of_Records);
            break;
        case 9:
            break;
        default:
            printf("\n No such choice, enter again\n");
            break;
        }
    }
    DeleteAVLTree(root_Of_Records); // To delete the AVL tree
    return 0;
}

// *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-* Student Database Operations' Definitions   -*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

void printGSRLutil(Tree_Node *root)
{
    if (root)
    {
        printGSRLutil(root->left);
        printf("\n%20s\t%s\t%d\t%s ", root->data.name, root->data.dep, root->data.year + 1, root->data.rollno);
        printGSRLutil(root->right);
    }
}
void printGSRL(Tree_Node *root)
{
    printGSRLutil(root);
}

void printAllNonApplicantStudentsutil(Tree_Node *root)
{
    if (root)
    {
        printAllNonApplicantStudentsutil(root->left);
        if (root->ptr_To_Applicant_Status_List_Node->apply == NOTAPPLIED)
        {
            printf("\n%20s \t%s\tApplicant Status-> %d", root->ptr_To_Applicant_Status_List_Node->name, root->ptr_To_Applicant_Status_List_Node->roll, root->ptr_To_Applicant_Status_List_Node->apply);
        }
        printAllNonApplicantStudentsutil(root->right);
    }
}
void printAllNonApplicantStudents(Tree_Node *root)
{
    printf("\nQuestion 2\n Non-Applicants");
    printAllNonApplicantStudentsutil(root);
}

void printAllEligibleStudentsutil(Tree_Node *root)
{
    if (root)
    {
        printAllEligibleStudentsutil(root->left);
        if (root->ptr_To_Attendance_List_Node->attendance_Percent > 75 && root->ptr_To_Fee_Status_List_Node->fee == APPLIED && root->ptr_To_Applicant_Status_List_Node->apply == APPLIED)
        {
            printf("\n%20s\t%s\tAttendance=  %f\tFee Status=>%d\tApplicant=%d", root->data.name, root->ptr_To_Attendance_List_Node->rollno,
                   root->ptr_To_Attendance_List_Node->attendance_Percent, root->ptr_To_Fee_Status_List_Node->fee, root->ptr_To_Applicant_Status_List_Node->apply);
        }
        printAllEligibleStudentsutil(root->right);
    }
}
void printAllEligibleStudents(Tree_Node *root)
{
    printf("\nQuestion 3\nEligible Candidates");
    printAllEligibleStudentsutil(root);
}

void printAllLowAttendanceStudentsutil(Tree_Node *root)
{
    if (root)
    {
        printAllLowAttendanceStudentsutil(root->left);
        if (root->ptr_To_Attendance_List_Node->attendance_Percent <= 75)
        {
            printf("\n%20s\t%s\tAttendance=  %f", root->data.name, root->ptr_To_Attendance_List_Node->rollno, root->ptr_To_Attendance_List_Node->attendance_Percent);
        }
        printAllLowAttendanceStudentsutil(root->right);
    }
}
void printAllLowAttendanceStudents(Tree_Node *root)
{
    printf("\nQuestion 4\n Low Attendance");
    printAllLowAttendanceStudentsutil(root);
}

Tree_Node *deleteStudentRecordutil(Tree_Node *root, char *key)
{
    if (root == NULL)
        return root;

    if (strcmp(key, root->data.rollno) < 0)
        root->left = deleteStudentRecordutil(root->left, key);
    else if (strcmp(key, root->data.rollno) > 0)
        root->right = deleteStudentRecordutil(root->right, key);
    else
    {
        if (root->right == NULL && root->left == NULL)
        {
            free(root);
            root = NULL;
        }

        else if (root->left != NULL && root->right == NULL)
        {
            Tree_Node *temp = root->left;
            root = root->left;
            free(temp);
        }

        else if (root->right != NULL && root->left == NULL)
        {
            Tree_Node *temp = root->right;
            root = root->right;
            free(temp);
        }

        else
        {
            Tree_Node *temp = FindMin(root->right);
            root->data = temp->data;
            root->right = deleteStudentRecordutil(root->right, temp->data.rollno);
        }
    }
    if (root == NULL)
        return root;

    root->height = 1 + max(height(root->left), height(root->right));

    int balance = Balance(root);

    // Left Left Case
    if (balance > 1 && Balance(root->left) >= 0)
        return RightRotate(root);

    // Right Right Case
    if (balance < -1 && Balance(root->right) <= 0)
        return LeftRotate(root);

    // Left Right Case
    if (balance > 1 && Balance(root->left) < 0)
    {
        root->left = LeftRotate(root->left);
        return RightRotate(root);
    }

    // Right Left Case
    if (balance < -1 && Balance(root->right) > 0)
    {
        root->right = RightRotate(root->right);
        return LeftRotate(root);
    }
    return root;
}
Tree_Node *deleteStudentRecord(Tree_Node *root)
{
    printf("\nQuestion 5");

    char name_deleteStudentRecord[ROLL_SIZE];
    printf("\nEnter roll no for deletion\n");
    scanf("%s", name_deleteStudentRecord);

    printf("\nBefore Deleting");
    printGSRL(root);
    root = deleteStudentRecordutil(root, name_deleteStudentRecord);
    printf("\nAfter Deleting");
    printGSRL(root);
    connector(root);
    return root;
}

void Question6util(Tree_Node *root)
{
    if (root)
    {
        Question6util(root->left);
        if (root->ptr_To_Attendance_List_Node->attendance_Percent > 75 && root->ptr_To_Fee_Status_List_Node->fee == NOTAPPLIED)
        {
            printf("\n%20s\t%s\t%f\tFee Status=>%d", root->data.name, root->ptr_To_Attendance_List_Node->rollno,
                   root->ptr_To_Attendance_List_Node->attendance_Percent, root->ptr_To_Fee_Status_List_Node->fee);
        }
        Question6util(root->right);
    }
}
void Question6(Tree_Node *root)
{
    printf("\nQuestion 6");
    Question6util(root);
}

int findDepIndex(char *dep)
{
    int c = dep[0], x = -1;
    switch (c)
    {
    case 'C': // CS or CV
        if (dep[1] == 'S')
            x = 0;
        else
            x = 1;
        break;
    case 'E':
        if (dep[1] == 'C')
            x = 2;
        else
            x = 3;
        break;
    case 'M':
        x = 4;
        break;
    default:
        printf("\nERROR !!! DEPARTMENT NOT FOUND\n");
        break;
    }
    return x;
}
void print_Dep_With_Most_Defaulters(int count_Of_Defaulters_Department[5])
{
    int maxcount = count_Of_Defaulters_Department[0];
    int idx = 0;
    for (int i = 1; i < 5; i++)
    {
        if (maxcount < count_Of_Defaulters_Department[i])
        {
            maxcount = count_Of_Defaulters_Department[i];
            idx = i;
        }
    }
    printf("\n\n\nMost Defaulters From :->");
    switch (idx)
    {
    case 0:
        printf("\tCS");
        break;
    case 1:
        printf("\tCV");
        break;
    case 2:
        printf("\tEC");
        break;
    case 3:
        printf("\tEE");
        break;
    case 4:
        printf("\tME");
        break;

    default:
        printf("\t Error dep doesnt exist");
        break;
    }
}
void printAllDefaulterStudentsutil(Tree_Node *root, int count_department_wise[5])
{
    if (root)
    {
        printAllDefaulterStudentsutil(root->left, count_department_wise);
        if (root->ptr_To_Fee_Status_List_Node->fee == PENDING || root->ptr_To_Attendance_List_Node->attendance_Percent <= 75)
        {
            printf("\n%20s\t%s\t%s\tAttendance = %f\tFeeStatus = %d", root->data.name, root->data.dep, root->data.rollno,
                   root->ptr_To_Attendance_List_Node->attendance_Percent, root->ptr_To_Fee_Status_List_Node->fee);
            int dep_Index = findDepIndex(root->data.dep);
            count_department_wise[dep_Index]++;
        }
        printAllDefaulterStudentsutil(root->right, count_department_wise);
    }
}
void printAllDefaulterStudents(Tree_Node *root)
{
    printf("\nQuestion 7\nDefaulter Students");
    int count_department_wise[5] = {0};
    printAllDefaulterStudentsutil(root, count_department_wise);
    printf("\n\n\n");
    for (int i = 0; i < SUBJECT_SIZE; i++)
    {
        printf("\tDEP_%d = %d", i, count_department_wise[i]);
    }
    print_Dep_With_Most_Defaulters(count_department_wise);
}

void optimised_Inorder(Tree_Node *root, char *key1, char *key2)
{
    if (root)
    {
        // root < key1
        if (strcmp(root->data.rollno, key1) < 0)
        {
            optimised_Inorder(root->right, key1, key2);
        }
        // root== key1
        else if (strcmp(root->data.rollno, key1) == 0)
        {
            printf("\n%20s\t%s", root->data.name, root->data.rollno);
            optimised_Inorder(root->right, key1, key2);
        }
        // root> key1 && root< key2
        else if (strcmp(root->data.rollno, key1) > 0 && strcmp(root->data.rollno, key2) < 0)
        {
            optimised_Inorder(root->left, key1, key2);
            printf("\n%20s\t%s", root->data.name, root->data.rollno);
            optimised_Inorder(root->right, key1, key2);
        }
        // root ==key2
        else if (strcmp(root->data.rollno, key2) == 0)
        {
            optimised_Inorder(root->left, key1, key2);
            printf("\n%20s\t%s", root->data.name, root->data.rollno);
        }
        // root>key2
        else
        {
            optimised_Inorder(root->left, key1, key2);
        }
    }
}
void printAllStudentsWithinRange(Tree_Node *root)
{
    printf("\nRange-search students based on the roll number.");

    char key1[ROLL_SIZE], key2[ROLL_SIZE];
    printf("\nEnter starting roll no for the operation\n");
    scanf("%s", key1);
    printf("\nEnter ending roll no for the operation\n");
    scanf("%s", key2);

    printf("\nstr1=%s str2=%s\n", key1, key2);
    optimised_Inorder(root, key1, key2);
}
