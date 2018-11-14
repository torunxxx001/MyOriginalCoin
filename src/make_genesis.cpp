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


#include <vector>
#include <string>

#include "chainparams.cpp"

const std::function<std::string(const char*)> G_TRANSLATION_FUN = nullptr;

CBlock generateGenesis(std::string networkID, uint32_t nTime, int32_t nVersion, const CAmount& genesisReward){
  SelectParams(networkID);
  printf("\n");

  unsigned int randNonce = 0;
  GetRandBytes((unsigned char*)&randNonce, sizeof(randNonce));
  randNonce = randNonce & 0xFFFF;

  arith_uint256 bnPowLimit = UintToArith256(Params().GetConsensus().powLimit);
  unsigned int powCompact = bnPowLimit.GetCompact();

  CBlock genesis;
  while(true){
    genesis = CreateGenesisBlock(nTime, randNonce, powCompact, nVersion, genesisReward);
    if(UintToArith256(genesis.GetHash()).GetCompact() < powCompact) break;

    randNonce++;
  }

  bool chk = CheckProofOfWork(genesis.GetHash(), genesis.nBits, Params().GetConsensus());
  printf("CHK: %d\n", (int)chk);

  return genesis;
}

int main(){
  RandomInit();

  CBlock genesis;
  CBlockIndex genesisIdx;
  CAmount genesisReward;

  arith_uint256 bnWork;

  printf("----MAIN----\n");
  genesisReward = 0;
  genesis = generateGenesis("main", time(NULL), 1, genesisReward);
  genesisIdx = CBlockIndex(genesis.GetBlockHeader());
  bnWork = GetBlockProof(genesisIdx);

  printf("GenesisHash: %s\n", genesis.GetHash().ToString().c_str());
  printf("MarkleRootHash: %s\n", genesis.hashMerkleRoot.ToString().c_str());
  printf("Nonce 0x%X\n", genesis.nNonce);
  printf("Bits: 0x%X\n", genesis.nBits);
  printf("CurWork: 0x%s\n", bnWork.GetHex().c_str());
  printf("Time: %d\n", genesis.nTime);
  printf("Reward: %ld\n", genesisReward);

  printf("----TEST----\n");
  genesisReward = 0;
  genesis = generateGenesis("test", time(NULL), 1, genesisReward);
  genesisIdx = CBlockIndex(genesis.GetBlockHeader());
  bnWork = GetBlockProof(genesisIdx);

  printf("GenesisHash: %s\n", genesis.GetHash().ToString().c_str());
  printf("MarkleRootHash: %s\n", genesis.hashMerkleRoot.ToString().c_str());
  printf("Nonce 0x%X\n", genesis.nNonce);
  printf("Bits: 0x%X\n", genesis.nBits);
  printf("CurWork: 0x%s\n", bnWork.GetHex().c_str());
  printf("Time: %d\n", genesis.nTime);
  printf("Reward: %ld\n", genesisReward);

  return 0;
}
