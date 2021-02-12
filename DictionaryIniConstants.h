#ifndef __DICT_INI_CONSTANTS_H__
#define __DICT_INI_CONSTANTS_H__

#include "data_types.h"

//////////////////////////////////////////////////////////////////
///PARAMETERS_SET.INI SECTIONS
//////////////////////////////////////////////////////////////////
const typo_string DICTIONARYINDEXCONSTANTS_SECTION=_T("DictionaryIndexConstants");
const typo_string ALG_CONSTANTS_SECTION=_T("AlgorithmsConstants");
const typo_string SYNTACTICSIMILARITYCONSTANTS_SECTION=_T("SyntacticSimilarityConstants");
const typo_string LETTERSMAPPINGRULESCONSTANTS_SECTION=_T("LettersMappingRulesConstants");
const typo_string NGRAMCONSTANTS_SECTION=_T("NGramConstants");
const typo_string SEMANTICSPELLERCONSTANTS_SECTION=_T("SemanticSpellerConstants");
const typo_string SYNTACTICSPELLERCONSTANT_SECTIONS=_T("SyntacticSpellerConstants");
const typo_string LETTERSETINTERSECTIONCONSTANTS_SECTION=_T("LetterSetIntersectionConstants");
const typo_string KEYBOARDRULESLISTCONSTANTS_SECTION=_T("KeyboardRulesListConstants");


//////////////////////////////////////////////////////////////////
///ALGORITHMS.INI SECTIONS
//////////////////////////////////////////////////////////////////
const typo_string LETTERSMAPPINGARRAYSSIZES_SECTION=_T("LettersMappingArraysSizes");
const typo_string ALGORITHMS_DICTIONARY_SECTION=_T("AlgorithmsDictionary");
const typo_string ALGORITHMS_TGOL=_T("AlgorithmsTargetGroupOrderedLists");
const typo_string CONTAINER_TO_COMPARISON_MAP_SECTION=_T("ContainerToComparisonMap");
const typo_string CONTAINER_TO_PROBLEMTYPE_MAP_SECTION=_T("ContainerToProblemTypeMap");
const typo_string MAPPER_CATALOG_SECTION=_T("MapperCatalogSection");

//////////////////////////////////////////////////////////////////
///[AlgorithmsConstants] KEY NAMES 
//////////////////////////////////////////////////////////////////
const typo_string MAXDICTSIZE=_T("MaxDictSize");
const typo_string MAXNGRAMSIZE=_T("MaxNGramSize");
const typo_string NGRAM_LENGTH=_T("NGram_Length");
const typo_string MINSIM=_T("MinSim");
const typo_string SPELLINGSSUGGESTIONSLISTSIZE=_T("SpellingsSuggestionsListSize");
const typo_string SPELLINGSSUGGESTIONSLISTSIZEMAX=_T("SpellingsSuggestionsListSizeMax");
const typo_string TOPSIM=_T("TopSim");
const typo_string ADJUSTWEIGHT=_T("AdjustWeight");
const typo_string COUNTPROBABILITIESFROMCANDIDATESLIST=_T("countProbabilitiesFromCandidatesList");
const typo_string DICTFREQWEIGHT=_T("DictFreqWeight");
//const typo_string EDITDISTWEIGHT=_T("EditDistWeight");
//const typo_string TOCALCULATEEDITDIST=_T("ToCalculateEditDist");
const typo_string TOCALCULATEFREQWEIGHT=_T("ToCalculateFreqWeight");
const typo_string WORDCOMPLETIONSUGGESTIONSLISTSIZE=_T("WordCompletionSuggestionsListSize");
const typo_string OPINIONMATTERS=_T("OpinionMatters");
const typo_string PREVCALCWEIGHT=_T("PrevCalcWeight");
const typo_string LINEARWEIGHT=_T("LinearWeight");
const typo_string LINEARLIMIT=_T("LinearLimit");
const typo_string SYNTLIMIT=_T("SyntLimit");
const typo_string SYNTACTICWEIGHT=_T("SyntacticWeight");
const typo_string LETTERSETWEIGHT=_T("LettersetWeight");
const typo_string COMPFUNCFORWEIGHT=_T("CompFuncForWeight");
const typo_string COMPFUNCAGAINSTWEIGHT=_T("CompFuncAgainstWeight");
const typo_string ZEROCOUNTERWEIGHT=_T("ZeroCounterWeight");
const typo_string SAMESIZEWEIGHT=_T("SameSizeWeight");
const typo_string DIFFSIZEWEIGHT=_T("DiffSizeWeight");
const typo_string SAMEFIRSTLETTER=_T("SameFirstLetter");
const typo_string SAMELASTLETTER=_T("SameLastLetter");
const typo_string DIFFLASTLETTER=_T("DiffLastLetter");
const typo_string SAMEPRELASTLETTER=_T("SamePreLastLetter");
const typo_string DIFFPRELASTLETTER=_T("DiffPreLastLetter");
const typo_string NGRAMSTARVALUE=_T("NGramStarValue");
const typo_string MEANINGFULFREQUENCY=_T("MeaningfulFrequency");
const typo_string NGRAMSTARMARGIN=_T("NGramStarMargin");
const typo_string SENTENCENGRAMINDEX_MIN_SIM=_T("SentenceNGramMinSim");
const typo_string SENTENCENGRAM_FOUND_COEFF=_T("SentenceNGramFoundCoeff");
const typo_string SENTENCENGRAM_NOTFOUND_COEFF=_T("SentenceNGramNotFoundCoeff");
const typo_string WARP=_T("Warp");
const typo_string SAFE_FALL=_T("Safe_Fall");
const typo_string SEMANTIC_ALG_FIXES_SYNTACTIC_ERRORS=_T("SemanticAlgorithmFixesSyntacticErrors");
const typo_string SYNONYM_BONUS=_T("SynonymBonus"); 
const typo_string MATCHES_WITHIN_WINDOW=_T("MatchesWithinWindow"); 
const typo_string FILTER_OUT_NON_SYNONYMS=_T("FilterOutNonSynonyms");
const typo_string TO_FIX_SEMANTIC_ERRORS=_T("ToFixSemanticErrors"); 
const typo_string TO_FIX_SYNTACTYC_ERRORS=_T("ToFixSyntacticErrors"); 
const typo_string RUN_SEMANTIC_ALGORITHM_TWICE=_T("RunSemanticAlgorithmTwice"); 
const typo_string MOST_LIKABLE_CANDIDATES_NUMBER=_T("most_likable_candidates_number"); 
const typo_string MINIMAL_MEANINGFUL_SUBSTRING_LENGTH=_T("minimal_meaningful_substring_length"); 
const typo_string MINIMAL_COMPOUND_WORD_SIZE=_T("minimal_compound_word_size"); 

const typo_string TO_SPELL_BY_SENTENCES=_T("ToSpellBySentences"); 
const typo_string TO_SPELL_WORDS_STREAM=_T("ToSpellWordsStream"); 


const typo_string NONVOWELSWEIGHT=_T("NonVowelsWeight");

const typo_string MAX_LENGTH_DIFF=_T("max_length_diff");
const typo_string MISPLACED_LETTER_WEIGHT=_T("misplaced_letter_weight");
const typo_string SYNTACTICSIMILARITYWEIGHT=_T("SyntacticSimilarityWeight");
const typo_string TOCALCULATESYNTACTICSIMILARITY=_T("ToCalculateSyntacticSimilarity");
const typo_string MIN_SIM_RATIO=_T("min_sim_ratio");



//const typo_string DELUTE_BY_COMPARE_TO_ORIGINAL1=_T("delute_by_compare_to_original1");
//const typo_string DELUTE_BY_COMPARE_TO_ORIGINAL2=_T("delute_by_compare_to_original2");
//const typo_string DELUTE_BY_COMPARE_TO_ORIGINAL3=_T("delute_by_compare_to_original3");

//////////////////////////////////////////////////////////////////
///[LETTERSMAPPINGRULESCONSTANTS_SECTION] KEY NAMES 
//////////////////////////////////////////////////////////////////

const typo_string CONTROL_WORD=_T("control_word");
const typo_string FIRST_LETTER_OPTIMIZATION=_T("first_letter_optimization");
const typo_string RECURSIONDEPTH=_T("recursiondepth");
const typo_string FILTER_OUT_LENGTH_FACTOR=_T("filter_out_length_factor");
//const typo_string TWO_ERRORS_IN_ONE_WORD=_T("two_errors_in_one_word");
const typo_string STARS_NUMBER_LIMIT1=_T("stars_number_limit1");
const typo_string STARS_NUMBER_LIMIT2=_T("stars_number_limit2");
const typo_string STARS_NUMBER_LIMIT3=_T("stars_number_limit3");
const typo_string OUTPUT_SIZE_LIMIT1=_T("output_size_limit1");
const typo_string OUTPUT_SIZE_LIMIT2=_T("output_size_limit2");
const typo_string EXISTING_WORD_BONUS=_T("existing_word_bonus");
const typo_string SUBSTITUTE_NGRAM_STAR_BONUS=_T("substitute_ngram_star_bonus");
const typo_string COMPARE_TO_ORIGINAL=_T("compare_to_original");
const typo_string SHORTEN_LIST=_T("shorten_list");
const typo_string FIRST_MATCH_IS_GOOD_MATCH=_T("first_match_is_good_match");
const typo_string TIME_LIMIT=_T("time_limit");
const typo_string RATE_BY_RECURSION_DEPTH=_T("rate_by_recursion_depth");
const typo_string ENCOUNTER_ONLY_EXISTING_WORDS=_T("encounter_only_existing_words");
const typo_string NARROWSEARCHBYNGRAM=_T("narrow_search_by_ngram");
const typo_string LEARNING_PHASE_WINDOW_SIZE=_T("learning_phase_window_size");
const typo_string USE_WORDNET=_T("use_wordnet");

//////////////////////////////////////////////////////////////////
//SYNTACTICSIMILARITYCONSTANTS_SECTION Parameters
//////////////////////////////////////////////////////////////////
const typo_string FULL_SIZE_BONUS=_T("full_size_bonus");
const typo_string FIRST_PART_BONUS=_T("first_part_bonus");
const typo_string MIDDLE_PART_BONUS=_T("middle_part_bonus");
const typo_string ENDING_BONUS=_T("ending_bonus");
const typo_string NON_COMPARABLE_BONUS=_T("non_comparable_bonus");
const typo_string LETTERSET_DIFFERENCE_COEFF=_T("letterset_difference_coeff");
const typo_string SAME_LETTERSET_BONUS=_T("same_letterset_bonus");
const typo_string SHIFT_MAX=_T("shift_max");
const typo_string SUBSTR_LENGTH_MAX=_T("substr_length_max");
const typo_string LENGTH_DIFFERENCE_COEFF=_T("length_difference_coeff");
const typo_string MIDDLE_PART_SCORE_ZERO_COEFF=_T("middle_part_score_zero_coeff");
const typo_string MIDDLE_AND_ENDING_TOO_LITTLE_COEFF=_T("middle_and_ending_too_little_coeff");
const typo_string MIDDLE_AND_ENDING_TOO_LITTLE=_T("middle_and_ending_too_little");
const typo_string DOUBLES_FULL_MATCH_BONUS=_T("doubles_full_match_bonus");

//////////////////////////////////////////////////////////////////
//LETTERSETINTERSECTIONCONSTANTS_SECTION Parameters
//////////////////////////////////////////////////////////////////

const typo_string ONLY_EXACT_MATCH_COUNTS=_T("only_exact_match_counts");
const typo_string MAX_MISSPELLINGS_NUMBER=_T("max_misspellings_number");



//////////////////////////////////////////////////////////////////
///[AlgorithmsDictionary] KEY NAMES
//////////////////////////////////////////////////////////////////
const typo_string CONTAINERS=_T("Containers");
const typo_string COMPARISON_FUNCTIONS=_T("ComparisonFunctions");
const typo_string LANGUAGES=_T("Languages");
const typo_string PROBLEM_TYPES=_T("ProblemTypes"); 
const typo_string TARGET_GROUPS=_T("TargetGroups");

//////////////////////////////////////////////////////////////////
//[AlgorithmsDictionary], Containers KEY VALUES
//////////////////////////////////////////////////////////////////
const typo_string DICTIONARY_INDEX=_T("DictionaryIndex");
const typo_string KEYBOARD_RULES_LIST=_T("KeyboardRulesList");
const typo_string LETTER_SET_INTERSECTION_LIST=_T("LetterSetIntersectionList");
const typo_string LETTERS_MAPPING_RULES_LIST=_T("LettersMappingRulesList");
const typo_string NGRAM_INDEX=_T("NGramIndex");
const typo_string SENTENCE_NGRAM_INDEX=_T("SentenceNGramIndex");
const typo_string SYNONYMS_MAP=_T("SynonymsMap");
const typo_string WORDS_MAPPING_RULES_LIST=_T("WordsMappingRulesList");

//////////////////////////////////////////////////////////////////
///[AlgorithmsDictionary], ComparisonFunctions KEY VALUES
//////////////////////////////////////////////////////////////////
const typo_string LINEAR_SIMILARITY=_T("LinearSimilarity");
const typo_string SYNTACTIC_SIMILARITY=_T("SyntacticSimilarity");
const typo_string NGRAM_SIMILARITY=_T("NGramSimilarity");
const typo_string LETTERSET_SIMILARITY=_T("LetterSetSimilarity");
//const typo_string LEVINSTEIN_EDIT_DISTANCE=_T("LevinsteinEditDistance");
const typo_string SEMANTICSIMILARITY=_T("SemanticSimilarity");

//////////////////////////////////////////////////////////////////
///[AlgorithmsDictionary], Languages KEY VALUES
//////////////////////////////////////////////////////////////////
const typo_string ISO639_1_EN=_T("en");
const typo_string ISO639_1_HE=_T("he");
const typo_string ISO639_1_DE=_T("de");
const typo_string ISO639_1_RU=_T("ru");

//////////////////////////////////////////////////////////////////
///[AlgorithmsDictionary], ProblemTypes KEY VALUES, NOT USED YET
//////////////////////////////////////////////////////////////////

//= RandomOmittedSpareLetters;RandomIncorrectLetter;LetterSwapping;AdjacentButtonHit;TypicalIncorrectSpelling;PhoneticWriting;WordMirroring;LetterMirroring;TypographicalLooksLike;DoubledWordDetection

//////////////////////////////////////////////////////////////////
///[AlgorithmsDictionary], TargetGroups KEY VALUES
//////////////////////////////////////////////////////////////////

const typo_string CHILDREN=_T("Children");
const typo_string DYSLECTICS=_T("Dyslectics");
const typo_string GENERAL=_T("General");
const typo_string LANGUAGE_LEARNERS=_T("LanguageLearners"); //ESL-students
const typo_string PROF_WRITERS=_T("ProfWriters");
const typo_string SPEED_TYPERS=_T("SpeedTypers");

//////////////////////////////////////////////////////////////////
///[AlgorithmsTargetGroupOrderedLists] KEY VALUES, NOT USED YET, ANYWAY ALREADY DEFINED ELSEWHERE
//////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
///[ContainerToProblemTypeMap]  NOT USED YET, ANYWAY ALREADY DEFINED ELSEWHERE
//////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
///[MapperCatalog] KEY NAMES
//////////////////////////////////////////////////////////////////
const typo_string SPELL_CHECKER_IMP_TYPES=_T("SpellCheckerImpTypes");
const typo_string SPELL_CHECKER_TYPES=_T("SpellCheckerTypes");

///////////////////////////////////// /////////////////////////////
///[MapperCatalog] KEY VALUES
//////////////////////////////////////////////////////////////////
const typo_string SPELLER=_T("Speller"); 
//const typo_string AUTO_MAP_TOOLKIT=_T("AutoMapToolkit");
//const typo_string IGNORE=_T("Ignore");
const typo_string SYNTACTIC_SPELLER_IMP=_T("SyntacticSpellerImp");
const typo_string SEMANTIC_SPELLER_IMP=_T("SemanticSpellerImp");
const typo_string SYN_SEM_SPELLER_IMP=_T("SyntacticoSemanticSpellerImp");

//////////////////////////////////////////////////////////////////
///[Path] KEY NAMES
//////////////////////////////////////////////////////////////////
const typo_string MAINCONFIGFILENAME=_T("MainConfigFileName");






















#endif //__DICT_INI_CONSTANTS_H__