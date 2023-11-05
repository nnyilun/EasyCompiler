#include "../include/parser.h"

/*
    使用模板元编程代替纯宏实现比较
*/
// 变量类型
#define TYPE_FIRST (Tag::KW_INT, Tag::KW_CHAR, Tag::KW_VOID)
// 表达式
#define EXPR_FIRST (Tag::LPAREN, Tag::NUM, Tag::CH, Tag::STR, Tag::ID, Tag::LNOT, Tag::SUB, Tag::FAO, Tag::MUL, Tag::INC, Tag::DEC)
// 语句
#define STATEMENT_FIRST (EXPR_FIRST, Tag::SEMICON, Tag::KW_WHILE, Tag::KW_FOR, Tag::KW_DO, Tag::KW_IF, Tag::KW_SWITCH, Tag::KW_RETURN, Tag::KW_BREAK, Tag::KW_CONTINUE)

Parser::Parser(Lexer& lexer) : _lexer(lexer) { }

template <Tag TagValue>
constexpr bool Parser::isTagEqual() {
    return unmatchToken->tag == TagValue;
}

template <Tag... Tags>
constexpr bool Parser::anyTagEqual() {
    return (isTagEqual<Tags>() || ...);
}

void Parser::getNextToken() {
    unmatchToken = _lexer.value().get().tokenize();
}

bool Parser::matchTag(Tag need) {
    if(unmatchToken->tag == need) {
        getNextToken();
        return true;
    } else return false;
}

void Parser::recovery(bool condition, SynError lost, SynError wrong) {
    // condition为true，则说明在给定的follow集合内；如果为false，则标记错误，读入下一个标记
    if(condition) {
        Error::synError(lost, unmatchToken);
    } else {
        Error::synError(wrong, unmatchToken);
        getNextToken();
    }
}


/*
	<program>  ->  <segment> <program> | ^
*/
void Parser::program() {
    if(isTagEqual<Tag::END>()) { // 分析结束
        return;
    } else {
        segment();
        program();
    }
}

/*
	<segment>  ->  rsv_extern <type> <def> | <type> <def>
*/
void Parser::segment() {

}

/*
	<init>  ->  assign <expr> | ^
*/
void Parser::init() {
    if(isTagEqual<Tag::ASSIGN>()) {
        expr();
    }
}



/*
	<statement>	 ->  <altexpr> semicon
                    | <whilestat> | <forstat> | <dowhilestat>
                    | <ifstat> | <switchstat>
                    | kw_break semicon
                    | kw_continue semicon
                    | kw_return <altexpr> semicon
*/
void Parser::statement() {
	switch(unmatchToken->tag) {
	case Tag::KW_WHILE: whilestat(); break;
	case Tag::KW_FOR: forstat(); break;
	case Tag::KW_DO: dowhilestat(); break;
	case Tag::KW_IF: ifstat(); break;
	case Tag::KW_SWITCH: switchstat(); break;
	case Tag::KW_BREAK:
		// ir.genBreak(); // 产生break语句
		getNextToken();
		if(!matchTag(Tag::SEMICON))
			recovery(anyTagEqual<TYPE_FIRST, STATEMENT_FIRST, Tag::RBRACE>(),
                SynError::SEMICON_LOST,SynError::SEMICON_WRONG);
		break;
	case Tag::KW_CONTINUE:
		// ir.genContinue(); // 产生continue语句
		getNextToken();
		if(!matchTag(Tag::SEMICON))
			recovery(anyTagEqual<TYPE_FIRST, STATEMENT_FIRST, Tag::RBRACE>(),
                SynError::SEMICON_LOST,SynError::SEMICON_WRONG);
		break;
	case Tag::KW_RETURN:
		getNextToken();
		// ir.genReturn(altexpr()); // 产生return语句
		if(!matchTag(Tag::SEMICON))
			recovery(anyTagEqual<TYPE_FIRST, STATEMENT_FIRST, Tag::RBRACE>(),
                SynError::SEMICON_LOST,SynError::SEMICON_WRONG);
		break;
	default:
		altexpr();
		if(!matchTag(Tag::SEMICON))
			recovery(anyTagEqual<TYPE_FIRST, STATEMENT_FIRST, Tag::RBRACE>(),
                SynError::SEMICON_LOST,SynError::SEMICON_WRONG);
	}
}

/*
	<whilestat>  ->  kw_while lparen <altexpr> rparen <block>
*/
void Parser::whilestat() {
    matchTag(Tag::KW_WHILE);
    if(!matchTag(Tag::LPAREN)){
        recovery(anyTagEqual<EXPR_FIRST, Tag::RPAREN>(), 
                SynError::LPAREN_LOST, SynError::LPAREN_WRONG);
    }
    // altexpr();
    if(!matchTag(Tag::RPAREN)) {
        recovery(anyTagEqual<Tag::LBRACE>(), SynError::RPAREN_LOST, SynError::RPAREN_WRONG);
    }
    // block();
}

void Parser::dowhilestat() { }
void Parser::forstat() { }
void Parser::forinit() { }
void Parser::ifstat() { }
void Parser::elsestat() { }
void Parser::switchstat() { }


void Parser::altexpr() { }

/*
	<expr>  ->  <assexpr>
*/
void Parser::expr() {
    
}

void Parser::analyze() {
    getNextToken();
    program();
}