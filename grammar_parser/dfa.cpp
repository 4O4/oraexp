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
    newStartRule->isStartRule=true;
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

    checkTransitions();

    qDebug() << "constructed LR(0) DFA in" << time.elapsed() << "ms";
}

void DFA::computeTransitions(DFAState *state)
{
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
    }
}

void DFA::checkTransitions()
{
    for(int i=0; i<states.size(); ++i){
        DFAState *state=states.at(i);
        QHash<QString,DFAState*> stateTransitions;
        for(int k=0; k<state->transitions.size(); ++k){
            DFATransition *transition=state->transitions.at(k);

            QString currentRuleItemLexeme = transition->sourceItem->currentRuleItem()->token.toString();
            DFAState *targetState=stateTransitions.value(currentRuleItemLexeme);
            if(targetState==0){
                stateTransitions[currentRuleItemLexeme]=transition->targetState;
            }else if(targetState!=transition->targetState){
                qDebug() << "State" << state->stateId << "has transitions on same symbol to different states";
                exit(1);
            }
        }
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
    computeLookaheadPropagations();
    propagateLookaheads();
    closeItems();
}

void DFA::computeLookaheadPropagations()
{
    int stateCount=states.size();
    for(int i=0; i<stateCount; ++i){
        DFAState *state=states.at(i);
        for(int k=0; k<state->dfaItems.size(); ++k){
            DFAItem *item=state->dfaItems.at(k);
            if(!item->isKernelItem() || item->isCompleteItem()){
                continue;
            }

            DFAState *tmpState=createTmpStateWithNonGrammarSymbol(item);
            closure_lalr1(tmpState);

            setLookaheadPropagations(state, tmpState, item);

            delete tmpState;
        }
    }

    printoutLookaheadsPropagationTable();
}

void DFA::setLookaheadPropagations(DFAState *state, DFAState *tmpState, DFAItem *keyItem)
{
    for(int i=0; i<tmpState->dfaItems.size(); ++i){
        DFAItem *item=tmpState->dfaItems.at(i);
        DFATransition *transition=state->findTransitionOnDFAItem(item);
        DFAState *targetState=transition->targetState;
        DFAItem *nextItem=findNextDFAItem(item);
        Q_ASSERT(nextItem);

        QList<EBNFToken> itemLookaheads = tmpState->lookaheads.value(item);
        for(int k=0; k<itemLookaheads.size(); ++k){
            const EBNFToken &lookahead = itemLookaheads.at(k);
            if(lookahead.tokenType==EBNFToken::NON_GRAMMAR){
                lookaheadsPropagationTable[qMakePair(state, keyItem)].append(qMakePair(targetState, nextItem));
            }else{
                targetState->addLookahead(nextItem, lookahead);
            }
        }
    }
}

void DFA::propagateLookaheads()
{
    EBNFToken eofToken;
    eofToken.tokenType=EBNFToken::E_O_F;
    eofToken.lexeme="$";
    states.at(0)->addLookahead(dfaItems.at(0), eofToken);

    bool hasChanges;

    do{
        hasChanges=false;

        QHashIterator<QPair<DFAState*, DFAItem*>,
                      QList< QPair<DFAState*, DFAItem* > > > i(lookaheadsPropagationTable);

        while (i.hasNext()) {
            i.next();

            QList<EBNFToken> sourceLookaheads = i.key().first->lookaheads.value(i.key().second);

            QList< QPair<DFAState*, DFAItem* > > targets = i.value();
            for(int j=0; j<targets.size(); ++j){
                QPair<DFAState*, DFAItem* > target=targets.at(j);
                DFAState *targetState=target.first;
                DFAItem *targetItem=target.second;

                for(int k=0; k<sourceLookaheads.size(); ++k){
                    bool added = targetState->addLookahead(targetItem, sourceLookaheads.at(k));
                    if(!hasChanges && added){hasChanges=true;}
                }
            }
        }
    }while(hasChanges);
}

void DFA::closeItems()
{
    for(int i=0; i<states.size(); ++i){
        DFAState *state=states.at(i);
        for(int k=0; k<state->dfaItems.size(); ++k){
            DFAItem *item=state->dfaItems.at(k);
            if(!item->isKernelItem() || item->isCompleteItem()){
                continue;
            }

            DFAState *tmpState=new DFAState();
            tmpState->addItem(item, true);
            tmpState->lookaheads[item]=state->lookaheads.value(item);
            closure_lalr1(tmpState);

            for(int l=0; l<tmpState->dfaItems.size(); ++l){
                DFAItem *tmpItem=tmpState->dfaItems.at(l);
                if(!tmpItem->isInitialItem()){
                    continue;
                }

                Q_ASSERT(state->contains(tmpItem));
                QList<EBNFToken> lookaheads=tmpState->lookaheads.value(tmpItem);
                //for(int m=0; m<lookaheads.size(); ++m){
                //    const EBNFToken &lookahead=lookaheads.at(m);
                //    state->addLookahead()
                //}
                state->lookaheads[tmpItem]=lookaheads;
            }

            delete tmpState;
        }
    }
}

DFAState *DFA::createTmpStateWithNonGrammarSymbol(DFAItem *item) const
{
    DFAState *state=new DFAState();
    state->addItem(item, true);
    state->addLookahead(item, createNonGrammarToken());

    return state;
}

void DFA::closure_lalr1(DFAState *state) const
{
    bool hasChanges;
    do{
        hasChanges=false;

        for(int i=0; i<state->dfaItems.size(); ++i){
            DFAItem *dfaItem=state->dfaItems.at(i);
            if(dfaItem->isCompleteItem() || dfaItem->currentRuleItem()->isTerminal){
                continue;
            }

            DFAItem *nextDFAItem=findNextDFAItem(dfaItem);
            BNFRuleItem *nextRuleItem=nextDFAItem->currentRuleItem();

            QString lookFor = dfaItem->currentRuleItem()->pointsTo;
            QList<DFAItem*> initItems=findAllInitialDFAItemsForRule(lookFor);
            for(int k=0; k<initItems.size(); ++k){
                DFAItem *initItem=initItems.at(k);
                if(!state->contains(initItem)){
                    state->addItem(initItem, false);
                    hasChanges=true;
                }

                bool nextDfaItemHasEpsilonInFirstSet=false;
                if(!nextDFAItem->isCompleteItem()){
                    if(nextRuleItem->isTerminal && !nextRuleItem->isEpsilon()){
                        bool added=state->addLookahead(initItem, nextRuleItem->token);
                        if(!hasChanges && added){hasChanges=true;}
                    }else if(!nextRuleItem->isTerminal && !nextRuleItem->isEpsilon()){
                        for(int m=0; m<nextDFAItem->rule->firstSet.size(); ++m){
                            const EBNFToken &firstSetToken=nextDFAItem->rule->firstSet.at(m);
                            if(firstSetToken.tokenType!=EBNFToken::EPSILON){
                                bool added=state->addLookahead(initItem, firstSetToken);
                                if(!hasChanges && added){hasChanges=true;}
                            }else{
                                nextDfaItemHasEpsilonInFirstSet=true;
                            }
                        }
                    }
                }else if(nextDFAItem->isCompleteItem() || nextDfaItemHasEpsilonInFirstSet){
                    QList<EBNFToken> currentLookaheads=state->lookaheads.value(dfaItem);
                    for(int m=0; m<currentLookaheads.size(); ++m){
                        const EBNFToken &currentLookahead=currentLookaheads.at(m);
                        Q_ASSERT(currentLookahead.tokenType!=EBNFToken::EPSILON);
                        bool added=state->addLookahead(initItem, currentLookahead);
                        if(!hasChanges && added){hasChanges=true;}
                    }
                }
            }
        }
    }while(hasChanges);
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

QList<DFAState *> DFA::findAllStatesWithDFAItem(DFAItem *dfaItem) const
{
    QList<DFAState *> results;

    for(int i=0; i<states.size(); ++i){
        DFAState *state=states.at(i);
        if(state->contains(dfaItem)){
            results.append(state);
        }
    }

    return results;
}

EBNFToken DFA::createNonGrammarToken() const
{
    EBNFToken token;
    token.lexeme="#";
    token.tokenType=EBNFToken::NON_GRAMMAR;

    return token;
}

void DFA::printoutLookaheadsPropagationTable()
{
    qDebug() << "----Lookaheads propagation table----";

    QHashIterator<QPair<DFAState*, DFAItem*>,
                  QList< QPair<DFAState*, DFAItem* > > > i(lookaheadsPropagationTable);
    while (i.hasNext()) {
        i.next();

        qDebug() << "S -" << i.key().first->stateId << " => " << i.key().second->toString(false) << ":";
        QList< QPair<DFAState*, DFAItem* > > targets = i.value();
        for(int i=0; i<targets.size(); ++i){
            QPair<DFAState*, DFAItem* > target=targets.at(i);
            qDebug() << "    S -" << target.first->stateId << " => " << target.second->toString(false);
        }
    }

    qDebug() << "--End lookaheads propagation table--";
}

void DFA::printoutDFA()
{
    qDebug() << "------LR(0) DFA states (" << states.size() << ")------";
    for(int i=0; i<states.size(); ++i){
        printoutState(states.at(i));
    }
    qDebug() << "---------------------------------";
}

void DFA::printoutState(DFAState *state)
{
    qDebug() << "---------";
    qDebug() << qPrintable(state->toString());
}
