#include "dfa.h"
#include "bnfrule.h"
#include "bnfruleitem.h"
#include "dfaitem.h"
#include "dfastate.h"
#include "dfatransition.h"
#include "firstfollowsetcomputer.h"
#include <QtDebug>
#include <QTime>

DFA::DFA(const QList<BNFRule*> &bnfRules) : bnfRules(bnfRules), stateCounter(0)
{
    if(!bnfRules.isEmpty()){
        augmentStartRule();
        FirstFollowSetComputer(this->bnfRules);
        generateDFAItems();
        constructDFAforLR0();
        constructDFAforLALR1();
        printoutDFA();
    }
}

DFA::~DFA()
{
    qDeleteAll(states);
    qDeleteAll(dfaItems);
    qDeleteAll(bnfRules);
}

DFAState *DFA::createDFAState()
{
    DFAState *state=new DFAState();
    state->stateId=stateCounter++;
    states.append(state);

    return state;
}

void DFA::augmentStartRule()
{
    BNFRule *currentStartRule=bnfRules.at(0);

    BNFRule *newStartRule=new BNFRule();
    newStartRule->ruleName=QString("%1_aug").arg(currentStartRule->ruleName);
    newStartRule->startAlternatives();

    BNFRuleItem *item=new BNFRuleItem();
    item->pointsTo=currentStartRule->ruleName;
    item->token.lexeme=currentStartRule->ruleName;
    item->token.tokenType=EBNFToken::ID;
    item->isTerminal=false;

    newStartRule->addRuleItem(item);

    bnfRules.insert(0, newStartRule);
}

void DFA::generateDFAItems()
{
    QTime time;
    time.start();

    int ruleCount=bnfRules.size();
    BNFRule *rule;
    QList < BNFRuleItem * > ruleItems;
    DFAItem *dfaItem;

    for(int i=0; i<ruleCount; ++i){
        rule=bnfRules.at(i);
        for(int k=0; k<rule->alternatives.size(); ++k){

            ruleItems=rule->alternatives.at(k);

            for(int j=0; j<=ruleItems.size(); ++j){
                dfaItem=new DFAItem();
                dfaItem->rule=rule;
                dfaItem->altIx=k;
                dfaItem->position=j;

                dfaItems.append(dfaItem);
            }
        }
    }

    qDebug() << "generated" << dfaItems.size() << " DFA items in" << time.elapsed() << "ms";
}

void DFA::constructDFAforLR0()
{
    QTime time;
    time.start();

    canonicalCollection();

    for(int i=0; i<states.size(); ++i){
        computeTransitions(states.at(i));
    }

    qDebug() << "constructed LR(0) DFA in" << time.elapsed() << "ms";
}

void DFA::computeTransitions(DFAState *state)
{
    //TODO: transitionlar duz hesablanmirlar: dragon book seh 244
    for(int i=0; i<state->dfaItems.size(); ++i){
        DFAItem *dfaItem=state->dfaItems.at(i);
        if(dfaItem->isCompleteItem()){
            continue;
        }

        QList<DFAItem*> targetItems=go_to(state->dfaItems, dfaItem->currentRuleItem());
        DFAState *targetState=hasStateWithItems(targetItems);
        Q_ASSERT(targetState);
        DFATransition *transition=new DFATransition(dfaItem, targetState);
        state->addTransition(transition);

        /*
        DFAItem *nextItem=findNextDFAItem(dfaItem);
        bool found=false;
        for(int k=state->stateId; k<states.size(); ++k){
            if(states.at(k)->contains(nextItem)){
                DFATransition *transition=new DFATransition(dfaItem, states.at(k));
                state->addTransition(transition);
                found=true;
                break;
            }
        }

        if(!found){
            for(int k=0; k<state->stateId-1; ++k){
                if(states.at(k)->contains(nextItem)){
                    DFATransition *transition=new DFATransition(dfaItem, states.at(k));
                    state->addTransition(transition);
                    break;
                }
            }
        }*/
    }
}

QList<DFAItem*> DFA::closure(QList<DFAItem*> items) const
{
    bool changed;

    QList<DFAItem*> results=items;

    DFAItem *dfaItem;
    BNFRuleItem *ruleItem;
    QList<DFAItem*> productions;
    DFAItem *prodDFAItem;

    do{
        changed=false;

        for(int i=0; i<results.size(); ++i){
            dfaItem=results.at(i);
            ruleItem=dfaItem->currentRuleItem();
            if(ruleItem==0 || ruleItem->isTerminal){ //complete item or terminal
                continue;
            }
            productions=findAllInitialDFAItemsForRule(ruleItem->pointsTo);
            for(int k=0; k<productions.size(); ++k){
                prodDFAItem=productions.at(k);

                //if it's epsilon, slide to complete item
                if(!prodDFAItem->isCompleteItem() && prodDFAItem->currentRuleItem()->isEpsilon()){
                    prodDFAItem=findNextDFAItem(prodDFAItem);
                }

                if(!results.contains(prodDFAItem)){
                    results.append(prodDFAItem);
                    changed=true;
                }
            }

        }
    }while(changed);

    return results;
}

QList<DFAItem*> DFA::go_to(QList<DFAItem*> items, BNFRuleItem *ruleItem)
{
    QList<DFAItem*> results;
    DFAItem *item;
    BNFRuleItem *currRuleItem;
    for(int i=0; i<items.size(); ++i){
        item=items.at(i);
        currRuleItem=item->currentRuleItem();
        if(currRuleItem==0 || currRuleItem->isEpsilon()){ //complete item
            continue;
        }

        if(*currRuleItem==*ruleItem){
            results.append(findNextDFAItem(item));
        }
    }

    results=closure(results);

    return results;
}

void DFA::canonicalCollection()
{
    QList<DFAItem*> firstItem;
    firstItem.append(dfaItems.at(0));
    QList<DFAItem*> cls=closure(firstItem);

    DFAState *state=createDFAState();
    state->dfaItems=cls;

    DFAState *tmpState;
    DFAItem *tmpItem;
    BNFRuleItem *tmpRuleItem;
    QList<DFAItem*> t;
    bool changed;
    do{
        changed=false;

        for(int i=0; i<states.size(); ++i){
            tmpState=states.at(i);
            for(int k=0; k<tmpState->dfaItems.size(); ++k){
                tmpItem=tmpState->dfaItems.at(k);
                tmpRuleItem=tmpItem->currentRuleItem();
                if(tmpRuleItem==0){ //complete item
                    continue;
                }

                t=go_to(tmpState->dfaItems, tmpRuleItem);
                if(!t.isEmpty() && !hasStateWithItems(t)){
                    state=createDFAState();
                    state->dfaItems=t;
                    changed=true;
                }
            }
        }

    }while(changed);

}

void DFA::constructDFAforLALR1()
{

}

QList<DFAItem*> DFA::findAllInitialDFAItemsForRule(const QString &ruleName) const
{
    QList<DFAItem*> results;
    DFAItem *item;
    for(int i=0; i<dfaItems.size(); ++i){
        item=dfaItems.at(i);
        if(item->pointsTo(ruleName) &&
                item->isInitialItem()){
            results.append(item);
        }
    }
    return results;
}

DFAItem *DFA::findDFAItem(BNFRule *rule, int altIx, int position) const
{
    DFAItem *item;
    for(int i=0; i<dfaItems.size(); ++i){
        item=dfaItems.at(i);
        if(item->rule==rule && item->altIx==altIx && item->position==position){
            return item;
        }
    }

    return 0;
}

DFAItem *DFA::findNextDFAItem(DFAItem *currItem) const
{
    return findDFAItem(currItem->rule, currItem->altIx, currItem->position+1);
}

DFAState* DFA::hasStateWithItems(QList<DFAItem*> items) const
{
    for(int i=0; i<states.size(); ++i){
        if(states.at(i)->equalsByItems(items)){
            return states.at(i);
        }
    }

    return 0;
}

void DFA::printoutDFA()
{
    qDebug() << "------LR(0) DFA states (" << states.size() << ")------";
    for(int i=0; i<states.size(); ++i){
        qDebug() << "---------";
        qDebug() << qPrintable(states.at(i)->toString());
    }
    qDebug() << "---------------------------------";
}
