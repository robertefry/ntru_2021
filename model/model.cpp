

#include <iostream>
#include "NTRU/NTRU.hh"

int main()
{
    ntru::NTRU_Init(0);

    std::cout << "~~~~~~~~  NTRU INIT  ~~~~~~~~~~~~~~~~~~~~~~~~~~~" << "\n";

    // ntru::NTRU_Seed<int> seed { 7, 2, 3, 41 };
    ntru::NTRU_Seed<int> seed { 11, 4, 13, 467 };
    std::cout << "Generating seed... " << (ntru::NTRU_IsValid(seed) ? "Valid!" : "")
        << "\n\t" << seed << "\n";

    ntru::NTRU_Basis<int> basis = ntru::NTRU_GenBasis(seed);
    std::cout << "Generating polynomial basis... " << (ntru::NTRU_IsValid(seed,basis) ? "Valid!" : "")
        << "\n\t" << basis << "\n";

    ntru::NTRU_KeyPair<int> keypair = ntru::NTRU_GenKeys(seed,basis);
    std::cout << "Generating key pairs... Done!" << "\n"
        << "\t" << "key_pub.poly_h  = " << keypair.key_pub.poly_h << "\n"
        << "\t" << "key_prv.poly_f  = " << keypair.key_prv.poly_f << "\n"
        << "\t" << "key_prv.poly_Fp = " << keypair.key_prv.poly_Fp << "\n";

    std::cout << "~~~~~~~~  NTRU ENCRYPT  ~~~~~~~~~~~~~~~~~~~~~~~~" << "\n";

    // ntru::Poly<int> message { 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0 };
    ntru::Poly<int> message { 3, 4, 5, 6, 7, 1, 2, 3 };
    std::cout << "message = " << message << "\n";

    ntru::Poly<int> cipher = ntru::NTRU_Encrypt(keypair.key_pub,message);
    std::cout << "Encrypting the message... Done!" << "\n"
        << "\t" << "cipher  = " << cipher << "\n";

    std::cout << "~~~~~~~~  NTRU DECRYPT  ~~~~~~~~~~~~~~~~~~~~~~~~" << "\n";

    std::cout << "cipher  = " << cipher << "\n";

    ntru::Poly<int> decrypt = ntru::NTRU_Decrypt(keypair.key_prv,cipher);
    std::cout << "Decrypting the message... Done!" << "\n"
        << "\t" << "decrypt  = " << decrypt << "\n";

    std::cout << "~~~~~~~ ~~~~~~~ ~~~~~~~ ~~~~~~~ ~~~~~~~ ~~~~~~~ " << "\n";
}
