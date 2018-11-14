#include "config/bitcoin-config.h"
#include "chainparams.h"
#include "key.h"
#include "blockencodings.h"
#include "random.h"
#include "base58.h"
#include "script/interpreter.h"
#include "policy/policy.h"
#include "pow.h"
#include "chain.h"

#include "key_io.h"

#include "outputtype.h"

#include <vector>
#include <string>

#include "chainparams.cpp"

const std::function<std::string(const char*)> G_TRANSLATION_FUN = nullptr;

void printFromSecret(const char* privKey){
  std::vector<unsigned char> genesis_privkey;
  genesis_privkey = ParseHex(privKey);

  CKey key;

  key.MakeNewKey(true);
  CPubKey pk = key.GetPubKey();
  printf("PublicKey: %s\n", HexStr(pk).c_str());

  std::string strAddr = EncodeSecret(key);
  printf("SecretAddress: %s\n", strAddr.c_str());

  CTxDestination dest;

  dest = GetDestinationForKey(pk, OutputType::LEGACY);

  strAddr = EncodeDestination(dest);

  printf("PublicAddress: %s\n", strAddr.c_str());
  printf("PubKeyHash: %s\n", HexStr(pk.GetID()).c_str());
  printf("\n\n");
}


int main(){
  RandomInit();
  ECC_Start();


  SelectParams("main");
  printf("\n\n");
  printFromSecret("");

  printf("\n\n");
  SelectParams("test");
  printf("\n\n");
  printFromSecret("");
  return 0;
}
