//
// #include <stdio.h>
// #include <math.h>

/*** 2 x 2 Matrices ***/

/* Pretty-prints the given matrix, with one line of text per row of matrix. */
void mat22Print(const double m[2][2]) {
    int i, j;
    for (i = 0; i < 2; i += 1) {
        for (j = 0; j < 2; j += 1)
            printf("%f    ", m[i][j]);
        printf("\n");
    }
}

/* Returns the determinant of the matrix m. If the determinant is 0.0, then the
matrix is not invertible, and mInv is untouched. If the determinant is not 0.0,
then the matrix is invertible, and its inverse is placed into mInv. The output
CANNOT safely alias the input. */
double mat22Invert(const double m[2][2], double mInv[2][2]) {
    double a,b,c,d,determinant;
    a = m[0][0];
    b = m[0][1];
    c = m[1][0];
    d = m[1][1];
    determinant = a*d - b*c;

    if (determinant == 0.0) {
        return determinant;
    } else {
        mInv[0][0] = 1/determinant * m[1][1];
        mInv[0][1] = 1/determinant * (-1) * m[0][1];
        mInv[1][0] = 1/determinant * (-1) * m[1][0];
        mInv[1][1] = 1/determinant * m[0][0];

        return determinant;
    }
}

/* Multiplies a 2x2 matrix m by a 2-column v, storing the result in mTimesV.
The output CANNOT safely alias the input. */
void mat221Multiply(const double m[2][2], const double v[2],
        double mTimesV[2]) {
    double m1 = m[0][0], m2 = m[0][1], m3 = m[1][0], m4 = m[1][1], v1 = v[0], v2 = v[1];

    mTimesV[0] = m1*v1 + m2*v2;
    mTimesV[1] = m3*v1 + m4*v2;
}

/* Fills the matrix m from its two columns. The output CANNOT safely alias the
input. */
void mat22Columns(const double col0[2], const double col1[2], double m[2][2]) {
    m[0][0] = col0[0];
    m[1][0] = col0[1];
    m[0][1] = col1[0];
    m[1][1] = col1[1];
}

/* The theta parameter is an angle in radians. Sets the matrix m to the
rotation matrix corresponding to counterclockwise rotation of the plane through
the angle theta. */
void mat22Rotation(double theta, double m[2][2]) {
    double cosTheta = cos(theta), sinTheta = sin(theta);
    m[0][0] = cosTheta;
    m[0][1] = (-1) * sinTheta;
    m[1][0] = sinTheta;
    m[1][1] = cosTheta;
}

/* Multiplies the 3x3 matrix m by the 3x3 matrix n. The output CANNOT safely
alias the input. */
void mat333Multiply(
        const double m[3][3], const double n[3][3], double mTimesN[3][3]) {
            int i, j, innerVal;
            for(i = 0; i < 3; i++){
                for(j = 0; j < 3; j++) {
                    for(innerVal = 0; innerVal < 3; innerVal++){
                        mTimesN[i][j] += m[i][innerVal] * n[innerVal][j];
                    }
                }
            }
        }

/* Multiplies the 3x3 matrix m by the 3x1 matrix v. The output CANNOT safely
alias the input. */
void mat331Multiply(
        const double m[3][3], const double v[3], double mTimesV[3]) {
            mTimesV[0] = m[0][0]*v[0] + m[0][1]*v[1] + m[0][2]*v[2];
            mTimesV[1] = m[1][0]*v[0] + m[1][1]*v[1] + m[1][2]*v[2];
            mTimesV[2] = m[2][0]*v[0] + m[2][1]*v[1] + m[2][2]*v[2];
        }

/* Builds a 3x3 matrix representing 2D rotation and translation in homogeneous
coordinates. More precisely, the transformation first rotates through the angle
theta (in radians, counterclockwise), and then translates by the vector t. */
void mat33Isometry(
    double theta, const double t[2], double isom[3][3]) {
        isom[0][0] = cos(theta);
        isom[0][1] = -sin(theta);
        isom[0][2] = t[0];
        isom[1][0] = sin(theta);
        isom[1][1] = cos(theta);
        isom[1][2] = t[1];
        isom[2][0] = 0;
        isom[2][1] = 0;
        isom[2][2] = 1;
}

/* Pretty-prints the given matrix, with one line of text per row of matrix. */
void mat33Print(const double m[3][3]) {
    int i, j;
    for (i = 0; i < 3; i += 1) {
        for (j = 0; j < 3; j += 1)
            printf("%f    ", m[i][j]);
        printf("\n");
    }
}
// 
// int main(void) {
//     double isom[3][3], t[2] = {2.0,2.0}, v[3] = {1,1,1}, vPrime[3], I[3][3] = {{1,0,0}, {0,1,0}, {0,0,1}}, test[3][3] = {{9,3,4}, {5,7,8}, {3,1,6}};
//     // mat33Isometry(3.14,t,isom);
//     // mat33Print(isom);
//     // mat331Multiply(isom,v,vPrime);
//     mat333Multiply(I, test, isom);
//     mat33Print(isom);
// }
