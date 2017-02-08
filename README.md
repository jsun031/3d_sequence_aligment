This is a 3 sequence alignment program. I try 2 versions: string-vector and char-int pointer.
To compare 3 sequences, you need to store them in 3 separate files on the same directory with the program.
Open the directory. Find doc1, doc2, doc3 in the main program and name the 3 values with the 3 files names.

For the string versions, I make it more objective oriented and depart the code into 4 files. StringInterval.h and StringInterval.cpp is the main data structure I use. compare.h and compare.cpp is the basic help function I write. Simple.cpp has 2 main alignment functions. One is the calculate the alignment score and the other is construct the alignment from beginning. 

How to compile?

In windows platform with a Visual Studio, try command "cl /EHsc main.cpp Simple.cpp StringInterval.cpp compare.cpp".

In linux platform, use "g++ -std=c++11 main.cpp Simple.cpp StringInterval.cpp compare.cpp.
	
After compiling and run with file NM_000558.fasta, NM_008218.fasta, and NM_013096.fasta, you would get a result that look like:

	NM_000558.fasta open correctly! length is 627.
	NM_008218.fasta open correctly! length is 569.
	NM_013096.fasta open correctly! length is 556.
	Total score is 4752.
	Lengths of alignment result sequences a, b, c are 631, 631, and 631.
	Perfect match number is 427.
	CATAAACCCTGGCGCGCTCGCGGCCCGGCACTCTTCTGGTCCCCACAGACTCAGAGAGAACCCACCATGGTGCTGTCTCCTGCCGACAAGACCAACGTCAAG_GCCGCCTGGGGTAAGGTCGGCGCGCACGCTGGCGAGTATGGTGCGGAGGCCCTGGAGAGGATGTTC_CTGTCCTTCCCCACCACCAAGACCTACTTCCCGCACTTCGACCTGAGCCACGGCTCTGCCCAGGTTAAGGGCCACGGCAAGAAGGTGGCCGACGCGCTGACCAACGCCG_TGGCGCACGTGGACGACATGCCCAACGCGCTGTCCGCCCTGAGCGACCTGCACGCGCACAAGCTTCGGGTGGACCCGGTCAACTTCAAGCTCCTAAGCCACTGCCTGCTGGTGACCCTGGCCGCCCACCTCCCCGCCGAGTTCACCCCTGCGGTGCACGCCTCCCTGGACAAGTTCCTGGCTTCTGTGAGCACCGTGCTGACCTCCAAATACCGTTAAGCTGGAGCCTCGGTGGCCATGCTTCTTGCCCCTTGGGCCTCCCCCCAGCCCCTCCTCCCCTTCC_TGCACCCGTACCCCCGTGGTCTTTGAATAAAGTCTGAGTGGGCGGCAAAAAAAAAAAAAAAAAAAAAA
	____GACACT_______T__________CTG_ATTCT_G_____ACAGACTC__AG_GAAGAAACCATGGTGCTCTCTGGGGAAGACAAAAGCAACATCAAG_GCTGCCTGGGGGAAGATTGGTGGCCATGGTGCTGAATATGGAGCTGAAGCCCTGGAAAGGATGTTTGCTA_GCTTCCCCACCACCAAGACCTACTTCCCTCACTTTGATGTAAGCCACGGCTCTGCCCAGGTCAAGGGTCACGGCAAGAAGGTCGCCGATGCTCTGGCCAATGCTGCAGGC_CACCTCGATGACCTGCCCGGTGCCCTGTCTGCTCTGAGCGACCTGCATGCCCACAAGCTGCGTGTGGATCCCGTCAACTTCAAGCTCCTGAGCCACTGCCTGCTGGTGACCTTGGCTAGCCACCACCCTGCCGATTTCACCCCCGCGGTGCATGCCTCTCTGGACAAATTCCTTGCCTCTGTGAGCACCGTGCTGACCTCCAAGTACCGTTAAGCT___GCCTT_CT_GCG__G_GGCTTG__CCTT____CT__GGCCATGCCCTTCTTCTCTCCCTTGCACCTGTA_CCTCTTGGTCTTTGAATAAAGCCTGAGT___AGG________AAGAAAAAAAAAAAA
	_____ACA_T_______T__________CTC_CTTCT_G_____ATAGACTC__AG_GAAGCAATCATGGTGCTCTCTGCAGATGACAAAACCAACATCAAGAACTG_CTGGGGGAAGATTGGTGGCCATGGTGGTGAATATGGCGAGGAGGCCCTACAGAGGATGTTCGCTG_CCTTCCCCACCACCAAGACCTACTTCTCTCACATTGATGTAAGCCCCGGCTCTGCCCAGGTCAAGGCTCACGGCAAGAAGGTTGCTGATGCCTTGGCCAAAGCTGCAGAC_CACGTCGAAGACCTGCCTGGTGCCCTGTCCACTCTGAGCGACCTGCATGCCCACAAACTGCGTGTGGATCCTGTCAACTTCAAGTTCCTGAGCCACTGCCTGCTGGTGACCTTGGCTTGCCACCACCCTGGAGATTTCACACCCGCCATGCACGCCTCTCTGGACAAATTCCTTGCCTCTGTGAGCACTGTGCTGACCTCCAAGTACCGTTAAGCC___GCCTC_CT_GCC__G_GGCTTG__CCTT____CT__GACCAGGCCCTTCTTCCCTCCCTTGCACCTATA_CCTCTTGGTCTTTGAATAAAGCCTGAGT___AGG________AAG___________C
	It tooks me 213.14 seconds.

For Char-int version, I just leave all the code in 1 file Align.cpp.

How to compile?

In windows platform with a Visual Studio, try command "cl /EHsc Align.cpp".

In linux platform, use "g++ -std=c++11 Align.cpp.
	
After compiling and run with file NM_000558.fasta, NM_008218.fasta, and NM_013096.fasta, you would get the result.

Comparison:

The algorithm I use is similar to string version, but I only use char array, char pointer, int array and int pointer instead of string and vector<int>. This makes the code a little longer than string version. But it runs 6 to 8 times faster than the string version. It seems that pointer is much faster than stl library. In practice, we might deal with sequences with 6000 characters. Then the stl version needs ten to twenty hours to calculate the result, but with char array version, it takes only two to three hours. The only tricky problem is to carefully allocate memory for the pointer and free it.
